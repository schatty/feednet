//
// Created by Igor Kuznetsov on 24/08/16.
//


/*
 * This is dangerous waters. Do not try to understand this code.
 */

#include <iostream>
#include "math.h"

#include "matrix_api.h"
#include <vector>

using namespace std;

MatrixXd rotate_matrix(MatrixXd &m, int shift_degree)
{
    MatrixXd m_polar = get_matrix_polar(m, 70, 360);


    //for (int i = 0; i < 2; ++i)
    //shift_left(m_polar);

    MatrixXd m_cartesian = get_matrix_cartesian(m_polar, m.rows(), m.cols());

    return m_cartesian;
}

MatrixXd get_matrix_polar(MatrixXd &matrix_catresian, int m_polar_rows, int m_polar_cols)
{
    double diag = sqrt(pow(matrix_catresian.rows(), 2) + pow(matrix_catresian.cols(), 2));
    cout << "DIAG: " << diag << endl;
    MatrixXd m_polar = MatrixXd::Zero(m_polar_rows, m_polar_cols);

    double angle_deg;
    double d;

    double phi_step = 360.0 / m_polar_cols;
    double r_step = diag / m_polar_rows;

    int offset_x = m_polar_cols / 2;
    int offset_y = m_polar_rows / 2;
    //cout << "Off: " << offset_x << " " << offset_y << endl;

    int Y = matrix_catresian.rows();
    int X = matrix_catresian.cols();

    cout << "1 phi step: " << phi_step << endl;
    cout << "1 r step: " << r_step << endl;

    for (int y = -offset_y + 1; y < offset_y; ++y)
        for (int x = -offset_x + 1; x < offset_x; ++x)
        {
            d = sqrt(pow(y, 2) + pow(x, 2));
            angle_deg = get_degrees(atan((double) y / (double) x));

            if (sin(get_radians(angle_deg)) < 0)
                cout << "sin < 0";
            if (cos(get_radians(angle_deg)) < 0)
                cout << "cos < 0";

            if (angle_deg >= 360)
                angle_deg = 359;

            int r_scaled = round(d / r_step);
            int phi_scaled = round(angle_deg / phi_step);

                /*
                if (r_scaled >= m_polar_rows)
                    r_scaled = m_polar_rows - 1;
                if (phi_scaled >= m_polar_cols)
                    phi_scaled = m_polar_cols - 1;
                    */
            cout << r_scaled << " " << phi_scaled << endl;

            m_polar(r_scaled, phi_scaled) = matrix_catresian(offset_y + Y - y - 1, offset_x + x);

        }

    cout << "end 1" << endl;
    return m_polar;
}

MatrixXd get_matrix_cartesian(MatrixXd &matrix_polar, int m_cart_rows, int m_cart_cols)
{
    MatrixXd m_cartesian = MatrixXd::Zero(m_cart_rows, m_cart_cols);

    double diag = sqrt(pow(m_cart_rows, 2) + pow(m_cart_cols, 2));
    double phi_step = 360.0 / matrix_polar.cols();
    double r_step = diag / matrix_polar.rows();


    int offset_x = matrix_polar.cols() / 2;
    int offset_y = matrix_polar.rows() / 2;
    //cout << "OFF: " << offset_x << " " << offset_y << endl;

    int Y = m_cart_rows;
    int X = m_cart_cols;

    cout << "2 phi step: " << phi_step << endl;
    cout << "2 r step: " << r_step << endl;

    double buf;
    int x, y;
    for (int r = 0; r < matrix_polar.rows(); ++r)
        for (int phi = 0; phi < matrix_polar.cols(); ++phi)
        {
            x = round(r * r_step * cos(get_radians(phi * phi_step))) + offset_x;
            y = round(r * r_step * sin(get_radians(phi * phi_step))) + offset_y;

            if (x < 0 || y < 0)
            {
                cout << x << " " << y << endl;
                continue;
            }

            if (x >= 0 && x < m_cart_cols && y >= 0 && y < m_cart_rows)
            {
                if (matrix_polar(r, phi) > m_cartesian(Y - y - 1, x) )
                {
                    m_cartesian(Y - y - 1, x) = matrix_polar(r, phi);
                }
            }
        }

    return m_cartesian;
}

void shift_left(MatrixXd &m)
{
    for (int i = 0; i < m.rows(); i++)
        m(i, m.rows() - 1) = m(i, 0);

    int i, j;
    for (i = 0; i < m.rows(); ++i)
        for (j = 0; j < m.cols() - 1; ++j)
            m(i, j) = m(i, j + 1);
}


double get_degrees(double rad)
{
    return rad * 180.0 / M_PI;
}

double get_radians(double dgr)
{
    return dgr * M_PI / 180.0;
}