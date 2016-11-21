//
// Created by Igor Kuznetsov on 29/07/16.
//

#include "Network.h"

#include <iostream>
#include "math.h"
#include <algorithm>
#include <iostream>

using namespace Eigen;
using namespace std;

VectorXd sigmoid(VectorXd z);
VectorXd sigmoid_prime(VectorXd z);
VectorXd softmax(VectorXd z);

default_random_engine generator;
double sample(double dummy)
{
    normal_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}


Network::Network(vector<int> sz)
{
    int i;

    num_layers = sz.size();

    // Set sizes
    sizes.resize(num_layers);
    for (i = 0; i < sz.size(); ++i)
        sizes(i) = sz[i];

    // Set biases
    for (i = 1; i < num_layers; ++i) 
    {
        biases.push_back(MatrixXd::Random(sizes(i), 1));
    }

    // Set weights
    for (i = 0; i < num_layers - 1; ++i)
    {
        weights.push_back(MatrixXd::Random(sizes[1+i], sizes[i]));
    }
}

VectorXd Network::feedforward(VectorXd a)
{
    for (int i = 0; i < biases.size(); i++)
    {
        a = sigmoid(weights[i] * a + biases[i]);
    }
    //cout << "Final activation: \n" << a << endl;

    return a;
}

void Network::SGD(vector<ex_pair> &training_data, int epochs, int mini_batch_size, double eta,
                    double test_data_rate, bool monitor_process)
{
    // number of images for testing
    int n_test = training_data.size() * test_data_rate;
    // number of images for training
    int n = training_data.size() - n_test;
    // shuffle data for randomly image-labels distribution between testing and training
    random_shuffle(training_data.begin(), training_data.end());

    // set iterator for the beginning of the training data
    vector<ex_pair>::iterator begin_train = training_data.begin();
    for (int i = 0; i < n_test; i++, begin_train++) ;

    // Reset progress accumulator
    progress.clear();

    for (int j = 0; j < epochs; ++j)
    {
        random_shuffle(begin_train, training_data.end());

        // update mini batches on the data within training part
        for (int k = n_test; k + mini_batch_size < n; k += mini_batch_size)
        {
            update_mini_batch(training_data, k, k + mini_batch_size, eta);
        }

        if (monitor_process)
            evaluate(training_data, 0, n_test);
    }

    // if monitor_process == false, evaluate testing only once at the end
    if (!monitor_process)
        evaluate(training_data, 0, n_test);
}

void Network::update_mini_batch(vector<ex_pair> &training_data, int ind_start, int ind_end, double eta)
{
    vector<MatrixXd> nabla_b;
    for (int i = 0; i < biases.size(); ++i)
        nabla_b.push_back(MatrixXd::Zero(biases[i].rows(), biases[i].cols()));

    vector<MatrixXd> nabla_w;
    for (int i = 0; i < weights.size(); ++i)
        nabla_w.push_back(MatrixXd::Zero(weights[i].rows(), weights[i].cols()));


    for (int i = ind_start; i < ind_end; ++i)
    {
        backprop(nabla_b, nabla_w, training_data[i]);
    }

    for (int i = 0; i < weights.size(); ++i)
    {
        weights[i] -= (eta / (ind_end - ind_start)) * nabla_w[i];
        biases[i] -= (eta / (ind_end - ind_start)) * nabla_b[i];
    }
}

void Network::backprop(vector<MatrixXd> &nabla_b, vector<MatrixXd> &nabla_w, ex_pair &ex)
{
    vector<MatrixXd> delta_nabla_b;
    for (int i = 0; i < biases.size(); ++i)
        delta_nabla_b.push_back(MatrixXd::Zero(biases[i].rows(), biases[i].cols()));

    vector<MatrixXd> delta_nabla_w;
    for (int i = 0; i < weights.size(); ++i)
        delta_nabla_w.push_back(MatrixXd::Zero(weights[i].rows(), weights[i].cols()));

    // feedforward
    VectorXd activation = ex.x;
    vector<VectorXd> activations;
    activations.push_back(ex.x);

    vector<VectorXd> zs;
    VectorXd z;
    VectorXd sp;

    for (int i = 0; i < weights.size(); ++i)
    {
        z = weights[i] * activation + biases[i];
        zs.push_back(z);
        activation = sigmoid(z);
        activations.push_back(activation);
    }

    // backward pass

    VectorXd delta = cost_derivative(activations.back(), ex.y);
    delta_nabla_b.back() = delta;
    delta_nabla_w.back() = delta * (activations[activations.size() - 2]).transpose();

    for (int l = 2; l < num_layers; l++)
    {
        z = zs[zs.size() - l];
        delta = (weights[weights.size() - l + 1].transpose() * delta);
        delta_nabla_b[delta_nabla_b.size() - l] = delta;
        delta_nabla_w[delta_nabla_w.size() - l] = delta * activations[activations.size() - l - 1].transpose();
    }

    // Add gradient deltas to the accumulator
    for (int i = 0; i < weights.size(); ++i)
    {
        nabla_b[i] += delta_nabla_b[i];
        nabla_w[i] += delta_nabla_w[i];
    }

}

VectorXd Network::cost_derivative(VectorXd output_activations, VectorXd y)
{
    return (output_activations - y);
}

void Network::evaluate(vector<ex_pair> &training_data, int test_data_start, int test_data_end)
{
    VectorXd a;
    int count = 0;
    for (int i = test_data_start; i < test_data_end; ++i)
    {
        int j = 0, j_max = 0;
        double cur_max = 0;
        a = feedforward(training_data[i].x);
        for (j = 0; j < a.size(); j++)
            if (a(j) > cur_max)
            {
                cur_max = a(j);
                j_max = j;
            }
        if (training_data[i].y[j_max])
            count++;
    }

    progress.push_back(count / (double) (test_data_end - test_data_start));
}

vector<double> Network::get_progress()
{
    return progress;
}

// Miscellaneous functions

VectorXd sigmoid(VectorXd z)
{
    VectorXd buf_vec = (VectorXd) ((ArrayXd) -z).exp();
    z = VectorXd::Ones(z.size()).cwiseQuotient(buf_vec + VectorXd::Ones(z.size()));
    return z;
}

VectorXd sigmoid_prime(VectorXd z)
{
    return sigmoid(z).cwiseProduct((VectorXd::Ones(z.size()) - sigmoid(z)));
}

VectorXd softmax(VectorXd z)
{
    double denom = ((ArrayXd) z).exp().sum();
    z = (VectorXd) (((ArrayXd) z).exp() / denom);
    return z;
}


// ***************************************************
// Network With Interface
// ***************************************************

NetworkWithInterface::NetworkWithInterface(vector<int> sz)
: Network(sz)
{
    weights_init_norm = false;
    cross_correlated_cost = false;
    softmax_on = false;
}

VectorXd NetworkWithInterface::feedforward(VectorXd a)
{
    for (int i = 0; i < biases.size(); i++)
    {
        if (softmax_on)
            a = softmax(weights[i] * a + biases[i]);
        else
            a = sigmoid(weights[i] * a + biases[i]);
    }
    //cout << "Final activation: \n" << a << endl;

    return a;
}

void NetworkWithInterface::SGD(vector<ex_pair> &training_data, int epochs, int mini_batch_size, double eta,
                  double test_data_rate, bool monitor_process)
{
    // number of images for testing
    int n_test = training_data.size() * test_data_rate;
    // number of images for training
    int n = training_data.size() - n_test;
    // shuffle data for randomly image-labels distribution between testing and training
    random_shuffle(training_data.begin(), training_data.end());

    // set iterator for the beginning of the training data
    vector<ex_pair>::iterator begin_train = training_data.begin();
    for (int i = 0; i < n_test; i++, begin_train++) ;

    // Reset progress accumulator
    progress.clear();

    for (int j = 0; j < epochs; ++j)
    {
        random_shuffle(begin_train, training_data.end());

        // update mini batches on the data within training part
        for (int k = n_test; k + mini_batch_size < n; k += mini_batch_size)
        {
            update_mini_batch(training_data, k, k + mini_batch_size, eta);
        }

        if (monitor_process)
            evaluate(training_data, 0, n_test);
    }

    // if monitor_process == false, evaluate testing only once at the end
    if (!monitor_process)
        evaluate(training_data, 0, n_test);
}

void NetworkWithInterface::update_mini_batch(vector<ex_pair> &training_data, int ind_start, int ind_end, double eta)
{
    vector<MatrixXd> nabla_b;
    for (int i = 0; i < biases.size(); ++i)
        nabla_b.push_back(MatrixXd::Zero(biases[i].rows(), biases[i].cols()));

    vector<MatrixXd> nabla_w;
    for (int i = 0; i < weights.size(); ++i)
        nabla_w.push_back(MatrixXd::Zero(weights[i].rows(), weights[i].cols()));


    for (int i = ind_start; i < ind_end; ++i)
    {
        backprop(nabla_b, nabla_w, training_data[i]);
    }

    for (int i = 0; i < weights.size(); ++i)
    {
        weights[i] -= (eta / (ind_end - ind_start)) * nabla_w[i];
        biases[i] -= (eta / (ind_end - ind_start)) * nabla_b[i];
    }
}

void NetworkWithInterface::backprop(vector<MatrixXd> &nabla_b, vector<MatrixXd> &nabla_w, ex_pair &ex)
{
    vector<MatrixXd> delta_nabla_b;
    for (int i = 0; i < biases.size(); ++i)
        delta_nabla_b.push_back(MatrixXd::Zero(biases[i].rows(), biases[i].cols()));

    vector<MatrixXd> delta_nabla_w;
    for (int i = 0; i < weights.size(); ++i)
        delta_nabla_w.push_back(MatrixXd::Zero(weights[i].rows(), weights[i].cols()));

    // feedforward
    VectorXd activation = ex.x;
    vector<VectorXd> activations;
    activations.push_back(ex.x);

    vector<VectorXd> zs;
    VectorXd z;
    VectorXd sp;

    for (int i = 0; i < weights.size(); ++i)
    {
        z = weights[i] * activation + biases[i];
        zs.push_back(z);
        if (softmax_on)
            activation = softmax(z);
        else
            activation = sigmoid(z);
        activations.push_back(activation);
    }

    // backward pass
    VectorXd delta;
    if (cross_correlated_cost)
        delta = cost_derivative(activations.back(), ex.y);
    else
        delta = cost_derivative(activations.back(), ex.y).cwiseProduct(sigmoid_prime(zs[zs.size()-1]));
    delta_nabla_b.back() = delta;
    delta_nabla_w.back() = delta * (activations[activations.size() - 2]).transpose();

    for (int l = 2; l < num_layers; l++)
    {
        z = zs[zs.size() - l];
        if (cross_correlated_cost)
        {
            delta = ( weights[weights.size() - l + 1].transpose() * delta );
        }
        else
        {
            sp = sigmoid_prime(z);
            delta = (weights[weights.size() - l + 1].transpose() * delta).cwiseProduct(sp);
        }
        delta_nabla_b[delta_nabla_b.size() - l] = delta;
        delta_nabla_w[delta_nabla_w.size() - l] = delta * activations[activations.size() - l - 1].transpose();
    }

    // Add gradient deltas to the accumulator
    for (int i = 0; i < weights.size(); ++i)
    {
        nabla_b[i] += delta_nabla_b[i];
        nabla_w[i] += delta_nabla_w[i];
    }

}

void NetworkWithInterface::reshape(vector<int> sz, bool weights_norm_on, bool cross_cost_on, bool soft_on)
{
    int i;

    // update net parameters
    weights_init_norm = weights_norm_on;
    cross_correlated_cost = cross_cost_on;
    softmax_on = soft_on;

    // Set sizes
    sizes.resize(sz.size());
    for (i = 0; i < sz.size(); ++i)
        sizes(i) = sz[i];

    // BIASES

    // Resize created matrix
    for (i = 1; i < sz.size() && i != num_layers; ++i)
    {
        biases[i - 1].resize(sizes(i), 1);
        biases[i - 1] = MatrixXd::Zero(sizes(i), 1).unaryExpr(ptr_fun(sample));
    }
    // If user extended net create new matrix
    for (; i < sz.size(); ++i)
    {
        biases.push_back(MatrixXd::Zero(sz[i], 1).unaryExpr(ptr_fun(sample)));
    }
    // If user shrinked net delete matrix
    for (--i;  biases.size() > sizes.size() - 1;)
    {
        biases.erase(biases.begin() + i);
    }

    // WEIGHTS

    // Resize created matrix
    for (i = 0; i < sz.size() - 1 && i != num_layers - 1; ++i)
    {
        weights[i].resize(sizes(1 + i), sizes[i]);
        weights[i] = MatrixXd::Zero(sizes(1 + i), sizes[i]).unaryExpr(ptr_fun(sample));

        // normalize weights
        if (weights_init_norm)
            weights[i] /= sqrt(sizes[i]);
    }
    // If user extended net create new matrix
    for (; i < sz.size() - 1; ++i)
    {
        weights.push_back(MatrixXd::Zero(sizes[1 + i], sizes[i]).unaryExpr(ptr_fun(sample)));
        // normalize weights
        if (weights_init_norm)
            weights.back() /= sqrt(sizes[i]);
    }
    // If user shrinked net - delete matrix
    for (; weights.size() > sizes.size() - 1; )
    {
        weights.erase(weights.begin() + i);
    }

    num_layers = sz.size();
}


