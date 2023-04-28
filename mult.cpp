//
// Created by Alex Gisi on 4/28/23.
//

#include "mult.h"

using namespace Eigen;


MatrixXd naive(const MatrixXd & A, const MatrixXd & B) {
    long int N = A.rows();

    MatrixXd C(N, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C(i, j) += A(i, k) * B(k, j);

    return C;
}
