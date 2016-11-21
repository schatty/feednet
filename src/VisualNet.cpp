//
// Created by Igor Kuznetsov on 09/08/16.
//

#include "VisualNet.h"
#include "math.h"
#include "gui_properties.h"
#include "MainWindow.h"

// ***************************************************
// Layer Icon
// ***************************************************

LayerIcon::LayerIcon(QWidget *parent, int layer_ind, int x, int y, int w, int h, string icon_path, int init_num_neurons,
                     bool mutable_size)
: QWidget(parent)
{
    is_mutable = mutable_size;
    num_neurons = init_num_neurons;
    ind = layer_ind;

    int side = w * 0.8;
    int btn_side = w * 0.45;
    icon = new SVGButton(this, w * 0.1, h - side - btn_side * 1.4, side, side, icon_path);

    // Label with number of neurons
    lbl_num_neurons = new QLabel(to_string(init_num_neurons).c_str(), this);
    lbl_num_neurons->move(w * 0.1, h - side - btn_side * 1.4);
    lbl_num_neurons->setFixedSize(side, side);
    lbl_num_neurons->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    lbl_num_neurons->setFont(font11);
    lbl_num_neurons->setStyleSheet(color_title_bold.c_str());


    // Increment button
    btn_inc = new PressedButton(this, 0, 0, btn_side, btn_side, ":img/plus_sign.svg", ":img/plus_sign_over.svg");
    connect(btn_inc, SIGNAL(clicked()), this, SLOT(increase_num_neurons()));

    // Decrement button
    btn_dec = new PressedButton(this, w * 0.55, 0, btn_side, btn_side, ":img/minus_sign.svg",
                                    ":img/minus_sign_over.svg");
    connect(btn_dec, SIGNAL(clicked()), this, SLOT(decrease_num_neurons()));

    // Delete button
    btn_delete = new PressedButton(this, w * 0.5 - btn_side/2, h - w * 0.5, btn_side, btn_side, ":img/delete.svg",
                                    ":img/delete_on.svg");
    connect(btn_delete, SIGNAL(clicked()), this, SLOT(delete_layer()));

    // Confirm size button
    btn_confirm_size = new PressedButton(this, w * 0.5 - btn_side/2, h - w * 0.5, btn_side, btn_side, ":img/ok.svg",
                                       ":img/ok.svg");
    connect(btn_confirm_size, SIGNAL(clicked()), this, SLOT(confirm_size()));
    btn_confirm_size->setVisible(false);

    le_size = new QLineEdit(this);
    le_size->move(0, h - side * 1.65);
    le_size->setFixedWidth(w);
    le_size->setFont(font11);
    le_size->setVisible(false);

    setGeometry(x, y, w, h);
}

void LayerIcon::change_geometry(int x, int y, int w, int h)
{
    setGeometry(x, y, w, h);

    int side = w * 0.8;
    int btn_side = w * 0.45;
    icon->move(w * 0.1, h - side - btn_side * 1.4);
    lbl_num_neurons->move(w * 0.1, h - side - btn_side * 1.4);
    if (is_mutable)
    {
        btn_inc->move(0, 0);
        btn_dec->move(w * 0.55, 0);
        btn_delete->move(w * 0.5 - btn_side/2, h - w * 0.5);
        btn_confirm_size->move(w * 0.5 - btn_side/2, h - w * 0.5);
        le_size->move(0, h - side * 1.65);
    }
}

void LayerIcon::init_icon(int x, int y, int num)
{

}

void LayerIcon::set_mutable(bool mtbl)
{
    is_mutable = mtbl;

    btn_inc->setVisible(mtbl);
    btn_dec->setVisible(mtbl);
    btn_delete->setVisible(mtbl);
}

void LayerIcon::set_num_neurons(int num)
{
    // Function emitted from the VisualNet, so it doesn't update VisualNet
    num_neurons = num;
    lbl_num_neurons->setText(to_string(num).c_str());
}

void LayerIcon::increase_num_neurons()
{
    if (num_neurons > 100)
        return;

    ++num_neurons;
    lbl_num_neurons->setText(to_string(num_neurons).c_str());
    ((VisualNet *) parentWidget())->change_net_structure(ind, num_neurons);
}

void LayerIcon::decrease_num_neurons()
{
    if (! (num_neurons - 1))
        return;

    --num_neurons;
    lbl_num_neurons->setText(to_string(num_neurons).c_str());
    ((VisualNet *) parentWidget())->change_net_structure(ind, num_neurons);
}

void LayerIcon::delete_layer()
{
    ((VisualNet *) parentWidget())->delete_layer(ind);
}

void LayerIcon::confirm_size()
{
    int new_size = le_size->text().toInt();
    if (new_size < 1 || new_size > 100)
    {
        ((VisualNet *) parentWidget())->send_message("Incorrect number of neurons", 2);
        return;
    }

    num_neurons = new_size;
    lbl_num_neurons->setText(to_string(num_neurons).c_str());
    le_size->setVisible(false);
    btn_confirm_size->setVisible(false);
    btn_inc->setEnabled(true);
    btn_dec->setEnabled(true);

    num_neurons = new_size;
    ((VisualNet *) parentWidget())->change_net_structure(ind, num_neurons);
}

void LayerIcon::mousePressEvent(QMouseEvent *e)
{
    if (! is_mutable)
        return;
    le_size->setVisible(true);
    le_size->setText(to_string(num_neurons).c_str());
    le_size->setFocus();
    le_size->selectAll();
    btn_confirm_size->setVisible(true);
    btn_inc->setEnabled(false);
    btn_dec->setEnabled(false);
}

void LayerIcon::paintEvent(QPaintEvent *e)
{
    if (is_mutable)
    {
        QPainter qp(this);
        qp.setBrush(color_light_gray);
        QPen pen(color_light_gray);
        pen.setWidth(1);
        qp.setPen(pen);
        qp.drawLine(width() * 0.5, width() * 0.5, width() * 0.5, height() - width() * 0.45);
    }
}


// ***************************************************
// Visual Net
// ***************************************************

double f(double x);

VisualNet::VisualNet(QWidget *parent, int x, int y, int w, int h)
: QWidget(parent)
{
    sizes = {28 * 28, 10, 5};
    num_layers = sizes.size();

    layer_icon_width = w * 0.07;
    space_between_layers = w * 0.02;
    setGeometry(x, y, w, h);

    // Create icons
    for (int i = 0; i < 10; ++i)
        icons[i] = new LayerIcon(this, i, 0, 0, layer_icon_width, layer_icon_width * 2, ":img/layer_icon.svg", 0, false);

    // Add new layer button
    int btn_s = w * 0.04;
    btn_add_layer = new PressedButton(this, 0, 0, btn_s, btn_s, ":img/plus_green.svg", ":img/plus_green_on.svg");
    connect(btn_add_layer, SIGNAL(clicked()), this, SLOT(add_new_layer()));

    // Panels with settings
    cb_activ_func = new CheckBoxesWidget(this, w * 0.03, h * 0.75, w * 0.3, h * 0.17,
                                         "ACTIVATION FUNCTION",
                                         {"SIGMOID", "SOFTMAX"});

    cb_cost = new CheckBoxesWidget(this, w * 0.35, h * 0.75, w * 0.3, h * 0.17,
                                   "COST FUNCTION",
                                   {"QUADRATIC", "CROSS-CORRELATED"});

    cb_weight_init = new CheckBoxesWidget(this, w * 0.67, h * 0.75, w * 0.3, h * 0.17,
                                          "WEIGHTS INIT.",
                                          {"N(μ,σ^2)", "N(μ,σ^2) / no. neurons"});
}



void VisualNet::add_new_layer()
{
    if (num_layers == 10)
    {
        ((MainWindow *) parentWidget())->set_message("No more 8 hidden layers, please", 2);
        return;
    }


    int num_output = sizes.back();
    sizes[sizes.size() - 1] = 10;
    sizes.push_back(num_output);

    init_net(sizes);
}

void VisualNet::delete_layer(int layer_id)
{
    if (sizes.size() == 3)
    {
        ((MainWindow *) parentWidget())->set_message("At least one hidden layer required", 2);
        return;
    }

    vector<int>::iterator p = sizes.begin();
    for (int i = 0; i < layer_id; p++, i++) ;

    sizes.erase(p);
    init_net(sizes);
}

void VisualNet::change_net_structure(int num_layer, int num_neurons)
{
    sizes[num_layer] = num_neurons;
}

void VisualNet::init_net(vector<int> new_sizes)
{
    sizes = new_sizes;
    num_layers = sizes.size();

    int step_height = height() * 0.1;
    mrg_x = (width() - ((num_layers+1) * layer_icon_width) - (num_layers) * space_between_layers ) / 2;

    double step_range01 = 1.0 / num_layers;
    for (int i = 0; i < num_layers - 1; ++i)
    {
        int add_height = height() * 0.5 * f(step_range01 * i);
        icons[i]->set_mutable((i != 0));
        icons[i]->change_geometry(mrg_x + i * (layer_icon_width + space_between_layers),
                              height() * 0.5 - add_height, layer_icon_width,
        layer_icon_width * 1.3 + add_height);
        icons[i]->set_num_neurons(sizes[i]);
        icons[i]->setVisible(true);
    }

    // Set last output icon layer separately
    icons[num_layers - 1]->change_geometry(mrg_x + num_layers * (layer_icon_width + space_between_layers),
                                       height() * 0.5 - layer_icon_width*2, layer_icon_width, layer_icon_width * 1.3 + layer_icon_width*2);
    icons[num_layers-1]->set_num_neurons(sizes[sizes.size() - 1]);
    icons[num_layers-1]->set_mutable(false);
    icons[num_layers-1]->setVisible(true);

    for (int i = num_layers; i < 10; ++i)
        icons[i]->setVisible(false);

    // + layer button
    btn_add_layer->move(mrg_x + (num_layers-1) * (layer_icon_width + space_between_layers) + (layer_icon_width-btn_add_layer->width())/2, height() * 0.5);
}

void VisualNet::send_message(char *text, int type)
{
    ((MainWindow *) parentWidget())->set_message(text, type);
}

void VisualNet::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    paint_background(qp, 0, 0, width(), height());

    // Line connecting layer icons
    QPen pen(color_light_gray);
    pen.setWidth(2);
    qp.setPen(pen);
    qp.drawLine(mrg_x + width() * 0.02, height() * 0.5 + width() * 0.02, width() - mrg_x - width() * 0.02, height() * 0.5 + width() * 0.02);
}

vector<bool> VisualNet::get_parameters()
{
    bool w_norm = cb_weight_init->get_checked_index();
    bool cross_corr = cb_cost->get_checked_index();
    bool softmax = cb_activ_func->get_checked_index();

    return {w_norm, cross_corr, softmax};
}

double f(double x)
{
    return pow(2.71, -3 * x);
}