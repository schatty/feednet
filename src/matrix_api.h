//
// Created by Igor Kuznetsov on 24/08/16.
//

#ifndef NN_INTERFACE_MATRIX_API_H
#define NN_INTERFACE_MATRIX_API_H

#include <Eigen/Core>

using namespace Eigen;

class matrix_api
{

};

MatrixXd rotate_matrix(MatrixXd &m, int shift_degree);

MatrixXd get_matrix_polar(MatrixXd &matrix_catresian, int m_polar_rows, int m_polar_cols);
MatrixXd get_matrix_cartesian(MatrixXd &matrix_polar, int m_cart_rows, int m_cart_cols);

void shift_left(MatrixXd &m);

// Miscellaneous functions

double get_degrees(double rad);
double get_radians(double dgr);

#endif //NN_INTERFACE_MATRIX_API_H
