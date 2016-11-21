//
// Created by Igor Kuznetsov on 06/08/16.
//

#ifndef NN_INTERFACE_VISUALIZATION_H
#define NN_INTERFACE_VISUALIZATION_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <vector>
#include <string>
#include <Eigen/Core>

#include "gui_widgets.h"

using namespace Eigen;
using namespace std;

// ***************************************************
// Hisgoram
// ***************************************************

class Histogram : public QWidget
{
    // Graphical stuff
    int margin_x, margin_y;
    int plot_width, plot_height;
    int num_bars;
    int bar_width;
    int bar_heights [15];
    bool data_initialized;
    int index_max_bar;

    QLabel *x_labels[15], *y_labels [5];
    QLabel *title;

public:
    Histogram(QWidget *parent, int x, int y, int w, int h, char *title_text);
    void init_histogram(vector<string> outputs);
    void set_histogram(VectorXd output_vals);

protected:
    void paintEvent(QPaintEvent *e);
};


// ***************************************************
// Plot
// ***************************************************

class Plot : public QWidget
{
protected:
    // Values range
    double x_lower, x_upper;
    double y_lower, y_upper;

    // Graphical stuff
    int margin_x, margin_y;
    int plot_width, plot_height;
    int num_labels_x, num_labels_y;
    QLabel *labels_x [10], *labels_y [10];
    TextButton *btn_title;

public:
    Plot(QWidget *parent, int x, int y, int w, int h,
         double x1, double x2, double y1, double y2,
         char *title, int title_x, int title_width);

protected:
    //virtual void paintEvent(QPaintEvent *e);

};


// ***************************************************
// Progress Plot
// ***************************************************

class ProgressPlot : public Plot
{
    // Graphic stuff
    int mrg_lines;

    int num_steps;
    QLabel *x_labels [3];
    QLabel *y_labels [5];
    vector<int> lines_heights;

    bool data_initialized;

public:
    ProgressPlot(QWidget *parent, int x, int y, int w, int h,
                 double x1, double x2, double y1, double y2,
                 char *title, int title_x, int title_width);
    void init_plot(vector<double> progress);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif //NN_INTERFACE_VISUALIZATION_H
