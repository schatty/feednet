//
// Created by Igor Kuznetsov on 28/07/16.
//

#include "gui_properties.h"
#include <QColor>

// FONTS

QFont font_title_bold11("Source Sans Pro", 11);
QFont font_title_bold12("Source Sans Pro", 12);
QFont font_title_bold13("Source Sans Pro", 13);
string color_title_bold("color: #58595B");

QFont font_common("Source Sans Pro", 11);
QFont font10("Source Sans Pro", 10);
QFont font11("Source Sans Pro", 11);
QFont font12("Source Sans Pro", 12);
QFont font13("Source Sans Pro", 13);

// COLOURS

QColor color_back(Qt::white);//218, 205, 187);
QColor color_dark_gray(200, 200, 200);
QColor color_light_gray(230, 230, 230);

QPen pen_border(QColor(200, 200, 200));

QColor color_plot_back(245, 245, 245);
QColor color_blue(51, 102, 153);
QColor color_calm_background(42, 70, 88, 30);

QColor color_green("#669966");

QColor color_green_transp(102, 153, 102, 190);
QColor color_blue_transp(51, 102, 153, 120);

QColor color_btn = color_light_gray;
QColor color_btn_on(220, 220, 220);


// Miscellaneous functions

void paint_background(QPainter &qp, int x, int y, int w, int h)
{
    int border_width = 1;
    qp.setPen(color_back);
    qp.setBrush(color_dark_gray);
    qp.drawRoundedRect(x, y, w, h, 3, 3);
    qp.setPen(color_dark_gray);
    qp.setBrush(color_back);
    qp.drawRoundedRect(x + border_width, y + border_width, x + w - border_width * 2, y + h - border_width * 2, 3, 3);
}

void init_gui_properties()
{
    font_title_bold11.setBold(true);
    font_title_bold12.setBold(true);
    font_title_bold13.setBold(true);
}