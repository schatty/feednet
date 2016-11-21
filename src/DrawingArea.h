//
// Created by Igor Kuznetsov on 28/07/16.
//

#ifndef NN_INTERFACE_DRAWINGAREA_H
#define NN_INTERFACE_DRAWINGAREA_H

#include <QWidget>
#include <QPainter>
#include <QComboBox>
#include <QStringList>

#include "gui_widgets.h"

class DrawingArea : public QWidget
{
    Q_OBJECT

    bool teach_mode;

    int x_left_widget;
    int y_up_widget;

    int side_widget;
    int side_grid;
    int grid_size;
    int margin;
    int cell_side;
    TextButton *btn_confirm;
    TextButton *btn_add;
    PressedButton *btn_clear;
    QComboBox *mode_box;
    QStringList *img_labels;

    // Move button
    PressedButton *btn_up;
    PressedButton *btn_down;
    PressedButton *btn_left;
    PressedButton *btn_right;
    // blur button
    PressedButton *btn_blur;
    PressedButton *btn_blur2x;

    // Fixed shift button
    ToggleButton *btn_fix_up;
    ToggleButton *btn_fix_down;
    ToggleButton *btn_fix_left;
    ToggleButton *btn_fix_right;
    // fix blur
    ToggleButton *btn_fix_blur;
    ToggleButton *btn_fix_blur2x;

    unsigned char grid_buf[31][31];

    QPushButton *btn;

public:
    DrawingArea(QWidget *parent, int x, int y, int widget_sd, int grid_sz);
    void set_grid(int new_grid_size);
    void set_teach_mode(int grid_sz, vector<string> *outputs);
    void set_recogn_mode();
    void save_grid();
    void load_grid_buf();
    string get_cur_lbl();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

private slots:
    void add_img();
    void clear_grid();
    void img_changed(int img_index);
    void recognize();

    void shift_img_up();
    void shift_img_down();
    void shift_img_left();
    void shift_img_right();
    void blur_img();
    void blur_img2x();
};


#endif //NN_INTERFACE_DRAWINGAREA_H
