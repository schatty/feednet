//
// Created by Igor Kuznetsov on 29/07/16.
//

#ifndef NN_INTERFACE_NETWORK_H
#define NN_INTERFACE_NETWORK_H

#include <vector>
#include <Eigen/Core>
#include <random>

#include "data_api.h"

using namespace std;
using namespace Eigen;


// ***************************************************
// Network
// ***************************************************

class Network
{
protected:
    int num_layers;
public:
    RowVectorXi sizes;
    vector<MatrixXd> biases;
    vector<MatrixXd> weights;
    vector<double> progress;

public:
    Network(vector<int> sz);
    VectorXd feedforward(VectorXd a);
    void SGD(vector<ex_pair> &training_data, int epochs, int mini_batch_size, double eta,
            double test_data_rate, bool watch_process);
    void update_mini_batch(vector<ex_pair> &training_data, int ind_start, int ind_end, double eta);
    void backprop(vector<MatrixXd> &nabla_b, vector<MatrixXd> &nabla_w, ex_pair &ex);
    VectorXd cost_derivative(VectorXd output_activations, VectorXd y);

    void evaluate(vector<ex_pair> &training_data, int test_data_start, int test_data_end);
    vector<double> get_progress();
};


// ***************************************************
// Network With Interface
// ***************************************************

class NetworkWithInterface : public Network
{
    bool cross_correlated_cost;
    bool weights_init_norm;
    bool softmax_on;

public:
    NetworkWithInterface(vector<int> sz);
    VectorXd feedforward(VectorXd a);
    void SGD(vector<ex_pair> &training_data, int epochs, int mini_batch_size, double eta,
             double test_data_rate, bool watch_process);
    void update_mini_batch(vector<ex_pair> &training_data, int ind_start, int ind_end, double eta);
    void backprop(vector<MatrixXd> &nabla_b, vector<MatrixXd> &nabla_w, ex_pair &ex);
    void reshape(vector<int> sz, bool weights_norm_on, bool cross_cost_on, bool soft_on);
};

#endif //NN_INTERFACE_NETWORK_H
