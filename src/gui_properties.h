//
// Created by Igor Kuznetsov on 28/07/16.
//

#ifndef NN_INTERFACE_GUI_PROPERTIES_H
#define NN_INTERFACE_GUI_PROPERTIES_H

#include <QFont>
#include <QPainter>
#include <QColor>
#include <string>

using namespace std;


extern QFont font_title_bold11;
extern QFont font_title_bold12;
extern QFont font_title_bold13;
extern string color_title_bold;
extern QFont font_common;
extern QFont font10;
extern QFont font11;
extern QFont font12;
extern QFont font13;

extern QColor color_back;
extern QColor color_dark_gray;
extern QColor color_light_gray;

extern QColor color_plot_back;
extern QColor color_blue;
extern QColor color_calm_background;
extern QColor color_green;

extern QColor color_green_transp;
extern QColor color_blue_transp;

extern QColor color_btn;
extern QColor color_btn_on;

extern QPen pen_border;

extern void paint_background(QPainter &qp, int x, int y, int w, int h);

void init_gui_properties();

#endif