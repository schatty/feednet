//
// Created by Igor Kuznetsov on 29/07/16.
//

#include "data_api.h"

#include <iostream>

using namespace std;

// ***************************************************
// dataset
// ***************************************************

dataset::dataset(string aname, vector<string> aoutput, int agrid_size, int acomplexity, string acomment)
{
    name = aname;
    //path= apath;
    output = aoutput;
    grid_size = agrid_size;
    complexity = acomplexity;
    comment = acomment;
    total_num = 0;
}

void dataset::init_new_ds(string aname, vector<string> aoutput, int agrid_size,
                          int acomplexity, string acomment)
{
    name = aname;

    output = aoutput;
    grid_size = agrid_size;
    complexity = acomplexity;
    comment = acomment;
    total_num = 0;
}

void dataset::add_image(string img_key, vector<unsigned char> img_rowdata)
{
    data.insert(pair<string, vector<unsigned char>>(img_key, img_rowdata));

    VectorXd x(img_rowdata.size());
    for (int i = 0; i < img_rowdata.size(); i++)
        x(i) = (double) 1.0 - (img_rowdata[i] / 255.0);
    VectorXd y = VectorXd::Zero(output.size());
    int pos = find(output.begin(), output.end(), img_key) - output.begin();
    y(pos) = 1;

    ds_pairs.push_back(ex_pair(x, y));
    total_num++;
}

multimap<string, vector<unsigned char>>* dataset::get_data()
{
    return &data;
}

int dataset::get_total_num_img()
{
    return total_num;
}

vector<ex_pair>* dataset::get_ds_in_pairs()
{
    return &ds_pairs;
}

vector<int> dataset::get_num_img_by_types()
{
    // ...
}