//
// Created by Alex Gisi on 4/28/23.
//

#ifndef MATRIXMULTIPLY_MULT_H
#define MATRIXMULTIPLY_MULT_H

#include <Eigen>

typedef Eigen::MatrixXd (*algFn)(const Eigen::MatrixXd &, const Eigen::MatrixXd &);

Eigen::MatrixXd naive(const Eigen::MatrixXd & A, const Eigen::MatrixXd & B);
Eigen::MatrixXd block(const Eigen::MatrixXd & A, const Eigen::MatrixXd & B);

#endif //MATRIXMULTIPLY_MULT_H
