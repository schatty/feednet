//
// Created by Igor Kuznetsov on 09/08/16.
//

#ifndef NN_INTERFACE_VISUALNET_H
#define NN_INTERFACE_VISUALNET_H

#include <QWidget>
#include <QLineEdit>
#include <QPainter>

#include "gui_widgets.h"

// ***************************************************
// Layer Icon
// ***************************************************

class LayerIcon : public QWidget
{
    Q_OBJECT

    int num_neurons;
    int ind;

    PressedButton *btn_inc, *btn_dec;
    PressedButton *btn_delete;
    PressedButton *btn_confirm_size;
    SVGButton *icon;
    QLabel *lbl_num_neurons;
    QLineEdit *le_size;
    bool is_mutable;

public:
    LayerIcon(QWidget *parent, int layer_ind, int x, int y, int w, int h, string icon_path, int init_num_neurons,
                bool mutable_size=true);
    void init_icon(int x, int y, int num);
    void hide_size_input();
    void change_geometry(int x, int y, int w, int h);

    // Setters
    void set_mutable(bool mtbl);
    void set_num_neurons(int num);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void increase_num_neurons();
    void decrease_num_neurons();
    void delete_layer();
    void confirm_size();
};


// ***************************************************
// Visual Net
// ***************************************************

class VisualNet : public QWidget
{
    Q_OBJECT

    vector<int> sizes;
    LayerIcon *icons [10];
    PressedButton *btn_add_layer;
    int num_layers;

    // Panels with settings
    CheckBoxesWidget *cb_activ_func;
    CheckBoxesWidget *cb_cost;
    CheckBoxesWidget *cb_weight_init;

    // Graphical stuff
    int layer_icon_width;
    int space_between_layers;
    int mrg_x;

public:
    VisualNet(QWidget *parent, int x, int y, int w, int h);
    void change_net_structure(int num_layer, int num_neurons);
    void init_net(vector<int> new_sizes);
    vector<int> get_sizes() { return sizes; }
    void delete_layer(int layer_id);
    void send_message(char *text, int type);

    // getter for nn-paramters. This function depends
    // from to the order of parameters, so be careful.
    // At the moment it returns: { weight_normalized flag, cross-correlated flag, softmax flag }
    vector<bool> get_parameters();

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void add_new_layer();
};


#endif //NN_INTERFACE_VISUALNET_H
