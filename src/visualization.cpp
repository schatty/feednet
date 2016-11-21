//
// Created by Igor Kuznetsov on 06/08/16.
//

#include "visualization.h"
#include "gui_properties.h"

#include <iostream>

// ***************************************************
// Histogram
// ***************************************************

Histogram::Histogram(QWidget *parent, int x, int y, int w, int h, char *title_text)
: QWidget(parent)
{
    margin_x = w * 0.07;
    margin_y = h * 0.1;

    plot_width = w * 0.85;
    plot_height = plot_width;

    // Title label
    title = new QLabel(this);
    title->setText(title_text);
    title->setFixedWidth(plot_width);
    title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    title->setStyleSheet(color_title_bold.c_str());
    title->setFont(font_title_bold12);

    // X-axis outputs label
    for (int i = 0; i < 15; i++)
    {
        x_labels[i] = new QLabel(this);
        x_labels[i]->setFont(font10);
        x_labels[i]->setVisible(false);
    }

    // Y-axis label
    int lbl_h = plot_height * 0.03;
    for (int i = 0; i < 5; ++i)
    {
        y_labels[i] = new QLabel(this);
        if (i != 4)
        y_labels[i]->setText(("0." + to_string(i * 25)).c_str());
        y_labels[i]->move(plot_width + margin_x * 0.5, plot_height - i * plot_height * 0.25 + margin_y - lbl_h);
        y_labels[i]->setFont(font10);
    }
    y_labels[4]->setText("1.0");

    data_initialized = false;

    setGeometry(x, y, w, h);
}

void Histogram::init_histogram(vector<string> outputs)
{
    num_bars = outputs.size();
    int i;
    int mrg_lbl = plot_width / 20;
    bar_width = (plot_width - mrg_lbl * 2) / num_bars;
    for (i = 0; i < num_bars; ++i)
    {
        x_labels[i]->setVisible(true);
        x_labels[i]->move( mrg_lbl + (i+1) * bar_width - bar_width / 2 , plot_height + margin_y * 1.2);
        x_labels[i]->setText(outputs[i].c_str());
        x_labels[i]->setFixedWidth(plot_width / 5);
    }
    for (; i < 15; ++i)
        x_labels[i]->setVisible(false);

    data_initialized = false;
}

void Histogram::set_histogram(VectorXd output_vals)
{
    double max_val = 0;
    index_max_bar = 0;
    for (int i = 0; i < num_bars; ++i) {
        if (output_vals(i) > max_val)
        {
            index_max_bar = i;
            max_val = output_vals(i);
        }
        bar_heights[i] = output_vals(i) * plot_height;
    }
    data_initialized = true;
    update();
}

void Histogram::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);

    // background
    qp.setPen(color_plot_back);
    qp.setBrush(color_plot_back);
    qp.drawRect(0, margin_y, plot_width, plot_height);

    // Axis lines
    for (int i = 0; i < 5; ++i)
    {
        // Draw semi-transparent line
        qp.setPen(QColor(200, 200, 200, 100));
        qp.drawLine(0, plot_height - i * plot_height * 0.25 + margin_y,
                    plot_width, plot_height - i * plot_height * 0.25 + margin_y);
        // Draw litlle line at the end
        qp.setPen(QColor(200, 200, 200));
        qp.drawLine(plot_width, plot_height - i * plot_height * 0.25 + margin_y,
                    margin_x * 0.4 + plot_width, plot_height - i * plot_height * 0.25 + margin_y);
    }

    // Bars
    int mrg = plot_width / 20;
    int dist = plot_width / 100;
    if (data_initialized)
    {
        for (int i = 0; i < num_bars; ++i)
        {
            if (i == index_max_bar)
            {
                qp.setPen(color_blue);
                qp.setBrush(color_blue);
            }
            else {
                qp.setPen(color_blue_transp);
                qp.setBrush(color_blue_transp);
            }
            qp.drawRoundedRect(mrg + i * bar_width + dist, plot_height - bar_heights[i] + margin_y, bar_width - dist * 2, bar_heights[i], 2, 2);
        }
    }

    qp.end();
}


// ***************************************************
// Plot
// ***************************************************

Plot::Plot(QWidget *parent, int x, int y, int w, int h,
           double x1, double x2, double y1, double y2,
           char *title_text, int title_x, int title_width)
: QWidget(parent)
{
    x_lower = x1;
    x_upper = x2;
    y_lower = y1;
    y_upper = y2;

    margin_x = h * 0.07;
    margin_y = h * 0.1;

    plot_width = w * 0.85;
    plot_height = h * 0.85;

    // Title label
    btn_title = new TextButton(this, title_x, 0, title_width, margin_y * 0.9, title_text,
                                "#f5f5f5");

    setGeometry(x, y, w, h);
}


// ***************************************************
// Progress Plot
// ***************************************************

ProgressPlot::ProgressPlot(QWidget *parent, int x, int y, int w, int h,
                           double x1, double x2, double y1, double y2,
                           char *title, int title_x, int title_width)
: Plot(parent, x, y, w, h, x1, x2, y1, y2, title, title_x, title_width)
{
    mrg_lines = plot_height * 0.05;

    // X-axis outputs label
    for (int i = 0; i < 3; i++)
    {
        x_labels[i] = new QLabel(this);
        x_labels[i]->setFont(font10);
        x_labels[i]->setVisible(false);
        x_labels[i]->setText("___");
    }
    x_labels[0]->move(mrg_lines / 2, margin_y * 1.2 + plot_height);
    x_labels[2]->move(plot_width - mrg_lines * 1.5, margin_y * 1.2 + plot_height);
    x_labels[1]->setText("EPOCHS");
    x_labels[1]->setVisible(true);
    x_labels[1]->move((plot_width - x_labels[1]->width()) / 2, margin_y * 1.2 + plot_height);

    // Y-axis label
    int lbl_h = plot_height * 0.03;
    for (int i = 0; i < 5; ++i)
    {
        y_labels[i] = new QLabel(this);
        if (i != 4)
            y_labels[i]->setText(("0." + to_string(i * 25)).c_str());
        y_labels[i]->move(plot_width + margin_x * 0.5, plot_height - i * plot_height * 0.25 + margin_y - lbl_h);
        y_labels[i]->setFont(font10);
    }
    y_labels[4]->setText("1.0");

    data_initialized = false;
}

void ProgressPlot::init_plot(vector<double> progress)
{
    num_steps = progress.size();

    x_labels[0]->setVisible(true);
    x_labels[0]->setText("1");
    x_labels[2]->setVisible(true);
    x_labels[2]->setText(to_string(num_steps).c_str());

    lines_heights.clear();
    for (int i = 0; i < num_steps; ++i)
    {
        lines_heights.push_back(progress[i] * plot_height);
    }

    data_initialized = true;
    update();
}

void ProgressPlot::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);

    // background
    qp.setPen(color_plot_back);
    qp.setBrush(color_plot_back);
    qp.drawRect(0, margin_y, plot_width, plot_height);

    // Draw axis lines
    qp.setPen(QColor(200, 200, 200));
    for (int i = 0; i < 5; ++i)
        qp.drawLine(plot_width, plot_height - i * plot_height * 0.25 + margin_y,
                    margin_x * 0.4 + plot_width, plot_height - i * plot_height * 0.25 + margin_y);

    // Draw progress
    int d = plot_height * 0.03;

    if (data_initialized)
    {
        // draw beautiful light background
        QPoint points [51];

        // calculate coordinates of the point to draw polygon
        int stp = num_steps > 50 ? 50 : num_steps;
        float step;
        if (stp >= 50) {
            step = num_steps / 50.0;
        } else
            step = 1;
        int space = plot_width / stp;
        mrg_lines = (plot_width - (stp - 1) * space) / 2;

        points[0] = QPoint(mrg_lines, margin_y + plot_height);
        for (int i = 0; i < stp; ++i)
            points[i + 1] = QPoint(mrg_lines + i * space, margin_y + plot_height - lines_heights[(int) (i * step)] + d/2);
        points[stp + 1] = QPoint(mrg_lines + (stp - 1) * space, margin_y + plot_height);

        qp.setPen(QColor(240, 240, 240, 200));
        qp.setBrush(color_calm_background);
        qp.drawPolygon(points, stp + 2);

        // Draw semi-transparent line
        for (int i = 0; i < 5; ++i)
        {
            qp.setPen(QColor(200, 200, 200, 100));
            qp.drawLine(0, plot_height - i * plot_height * 0.25 + margin_y,
                        plot_width, plot_height - i * plot_height * 0.25 + margin_y);

        }

        for (int i = 0; i < stp; ++i)
        {
            // Draw litlle line at the end
            qp.setPen(QColor(200, 200, 200));
            qp.drawLine(mrg_lines + i * space, margin_y + plot_height - lines_heights[(int) i * step],
                        mrg_lines + i * space, margin_y + plot_height);

            points[i + 1] = QPoint(mrg_lines + i * space, margin_y + plot_height - lines_heights[(int) i * step]);

            // circle at the end
            qp.setPen(QColor(255, 255, 255));
            qp.setBrush(color_blue);
            qp.drawEllipse(mrg_lines + i * space - d / 2, margin_y + plot_height - lines_heights[(int) i * step], d, d);
        }
    }
    qp.end();
}