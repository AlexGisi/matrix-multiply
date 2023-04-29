//
// Created by Alex Gisi on 4/28/23.
//

#include <cilk/cilk.h>
#include "mult.h"

using namespace Eigen;


MatrixXd naive(const MatrixXd & A, const MatrixXd & B) {
    long int N = A.rows();

    MatrixXd C = MatrixXd::Zero(N, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C(i, j) += A(i, k) * B(k, j);

    return C;
}

MatrixXd block(const MatrixXd & A, const MatrixXd & B) {
    int N = A.rows();
    int block_size = 16;
    int n_blocks = N / block_size;

    MatrixXd C = MatrixXd::Zero(N, N);
    for (int bi = 0; bi < n_blocks; bi++)
        for (int bj = 0; bj < n_blocks; bj++)
            for (int bk = 0; bk < n_blocks; bk++)
                for (int i = 0; i < block_size; i++)
                    for (int j = 0; j < block_size; j++)
                        for (int k = 0; k < block_size; k++)
                            C(bi*block_size + i, bj*block_size+j) += A(bi*block_size+i, bk*block_size+k) * B(bk*block_size+k, bj*block_size+j);

    return C;
}
