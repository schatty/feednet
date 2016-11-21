//
// Created by Igor Kuznetsov on 29/07/16.
//

#ifndef NN_INTERFACE_DATA_API_H
#define NN_INTERFACE_DATA_API_H

#include <string>
#include <vector>
#include <map>
#include <Eigen/Core>

#include "DSPreviewWidget.h"
using namespace Eigen;

using namespace std;

struct ex_pair
{
    ex_pair(VectorXd xa, VectorXd ya)
    {
        x = xa;
        y = ya;
    }

    VectorXd x;
    VectorXd y;
};

class dataset
{
public:
    string name;
    string path;
    multimap<string, vector<unsigned char>> data;
    vector<string> output;
    int complexity;
    int grid_size;
    string comment;
    int total_num;

    vector<ex_pair> ds_pairs;

public:
    dataset() { }
    dataset(string aname, vector<string> aoutput, int agrid_size, int acomplexity, string acomment = "");
    void init_new_ds(string aname, vector<string> aoutput, int agrid_size, int acomplexity, string acomment = "");
    void add_image(string img_key, vector<unsigned char> img_rowdata);

    // Getters
    string get_name() { return name; }
    //string get_path() { return path; }
    multimap<string, vector<unsigned char>> *get_data();
    int get_grid_size() { return grid_size; }
    int get_complexity() { return complexity; }
    string get_desc() { return comment; }
    vector<string> get_outputs() { return output; }
    int get_total_num_img();
    vector<int> get_num_img_by_types();

    vector<ex_pair> *get_ds_in_pairs();

    friend class DSPreviewWidget;
};

#endif