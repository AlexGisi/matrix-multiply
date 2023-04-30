//
// Created by Alex Gisi on 4/28/23.
//

#include <cilk/cilk.h>
#include "mult.h"

#define NAIVE_THRESHOLD 32
// #define ERR

using namespace Eigen;


#ifdef ERR
#include <iostream>
#include <ctime>
#include <chrono>

void printOp(int i, int j, int k) {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&now_time_t);
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);
    std::cerr << buffer
        << ": " << "C(" << i << ',' << j << ") += A(" << i << ',' << k
        << ") * B(" << k << ',' << j << ')' << std::endl;
}
#endif

MatrixXd naive(const MatrixXd & A, const MatrixXd & B) {
    long int N = A.rows();

    MatrixXd C = MatrixXd::Zero(N, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++) {
                C(i, j) += A(i, k) * B(k, j);
#ifdef ERR
                printOp(i, j, k);
#endif
            }

    return C;
}

MatrixXd block(const MatrixXd & A, const MatrixXd & B) {
    int N = A.rows();
    int block_size = 16;
    int n_blocks = N / block_size;

    MatrixXd C = MatrixXd::Zero(N, N);
    cilk_for (int bi = 0; bi < n_blocks; bi++)
        cilk_for (int bj = 0; bj < n_blocks; bj++)
            for (int bk = 0; bk < n_blocks; bk++)
                for (int i = 0; i < block_size; i++)
                    for (int j = 0; j < block_size; j++)
                        for (int k = 0; k < block_size; k++) {
                            C(bi*block_size + i, bj*block_size+j) += A(bi*block_size+i, bk*block_size+k) * B(bk*block_size+k, bj*block_size+j);
#ifdef ERR
                            printOp(i, j, k);
#endif
                        }

    return C;
}

void dac(const MatrixXd & A, const MatrixXd& B, MatrixXd& C,
         int x, int ic, int jc, int ia, int ja, int ib, int jb) {

    if (x <= NAIVE_THRESHOLD) {
        for (int i=0; i < x; i++)
            for (int j = 0; j < x; j++)
                for (int k = 0; k < x; k++) {
                    C(ic+i, jc+j) += A(ia+i, ja+k) * B(ib+k, jb+j);
#ifdef ERR
                    printOp(i, j, k);
#endif
                }
    } else {
        int y = x / 2;

        cilk_spawn dac(A, B, C, y, ic, jc, ia, ja, ib, jb);
        cilk_spawn dac(A, B, C, y, ic, jc+y, ia, ja, ib, jb+y);
        cilk_spawn dac(A, B, C, y, ic+y, jc, ia+y, ja, ib, jb);
        dac(A, B, C, y, ic+y, jc+y, ia+y, ja, ib, jb+y);
        cilk_sync;
        cilk_spawn dac(A, B, C, y, ic, jc, ia, ja+y, ib+y, jb);
        cilk_spawn dac(A, B, C, y, ic, jc+y, ia, ja+y, ib+y, jb+y);
        cilk_spawn dac(A, B, C, y, ic+y, jc, ia+y, ja+y, ib+y, jb);
        dac(A, B, C, y, ic+y, jc+y, ia+y, ja+y, ib+y, jb+y);
        cilk_sync;
    }
}

MatrixXd dac(const MatrixXd & A, const MatrixXd & B) {
    MatrixXd C = MatrixXd::Zero(A.rows(), A.rows());
    dac(A, B, C, A.rows(), 0, 0, 0, 0, 0, 0);

    return C;
}

void strassen_helper(const MatrixXd &A, const MatrixXd &B, MatrixXd &C,
                     int row_a, int col_a, int row_b, int col_b, int n) {
    if (n <= NAIVE_THRESHOLD) {
        C = naive(A, B);
        return;
    }

    int half = n / 2;

    MatrixXd A11 = A.block(row_a, col_a, half, half);
    MatrixXd A12 = A.block(row_a, col_a + half, half, half);
    MatrixXd A21 = A.block(row_a + half, col_a, half, half);
    MatrixXd A22 = A.block(row_a + half, col_a + half, half, half);

    MatrixXd B11 = B.block(row_b, col_b, half, half);
    MatrixXd B12 = B.block(row_b, col_b + half, half, half);
    MatrixXd B21 = B.block(row_b + half, col_b, half, half);
    MatrixXd B22 = B.block(row_b + half, col_b + half, half, half);

    MatrixXd P1(half, half);
    MatrixXd P2(half, half);
    MatrixXd P3(half, half);
    MatrixXd P4(half, half);
    MatrixXd P5(half, half);
    MatrixXd P6(half, half);
    MatrixXd P7(half, half);

    strassen_helper(A11 + A22, B11 + B22, P1, 0, 0, 0, 0, half);
    strassen_helper(A21 + A22, B11, P2, 0, 0, 0, 0, half);
    strassen_helper(A11, B12 - B22, P3, 0, 0, 0, 0, half);
    strassen_helper(A22, B21 - B11, P4, 0, 0, 0, 0, half);
    strassen_helper(A11 + A12, B22, P5, 0, 0, 0, 0, half);
    strassen_helper(A21 - A11, B11 + B12, P6, 0, 0, 0, 0, half);
    strassen_helper(A12 - A22, B21 + B22, P7, 0, 0, 0, 0, half);

    C.block(0, 0, half, half) = P1 + P4 - P5 + P7;
    C.block(0, half, half, half) = P3 + P5;
    C.block(half, 0, half, half) = P2 + P4;
    C.block(half, half, half, half) = P1 - P2 + P3 + P6;
}

MatrixXd strassen(const MatrixXd &A, const MatrixXd &B) {
    int n = A.rows();
    MatrixXd C(n, n);
    strassen_helper(A, B, C, 0, 0, 0, 0, n);
    return C;
}
