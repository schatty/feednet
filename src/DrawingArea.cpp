//
// Created by Igor Kuznetsov on 28/07/16.
//

#include <QApplication>
#include "DrawingArea.h"
#include "MainWindow.h"
#include <iostream>

#include "matrix_api.h"
#include "gui_properties.h"

using namespace std;

DrawingArea::DrawingArea(QWidget *parent, int x, int y, int widget_sd, int grid_sz) : QWidget(parent)
{
    teach_mode = false;

    x_left_widget = x;
    y_up_widget = y;

    side_widget = widget_sd;
    side_grid = (int) (side_widget * 0.8);
    grid_size = grid_sz;
    cell_side = side_grid / grid_size;
    margin = (side_grid - (cell_side * grid_size)) / 2;
    setGeometry(x, y, side_widget, side_widget);

    // Arrow left & fixed modification
    btn_left = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.03, 18, 18,
                               ":img/left.svg",
                               ":img/left_on.svg");
    connect(btn_left, SIGNAL(clicked()), this, SLOT(shift_img_left()));

    btn_fix_left = new ToggleButton(this, widget_sd * 0.89, widget_sd * 0.03, 20, 20,
                                  ":img/fixUpOff.svg",
                                  ":img/fixUpOn.svg");
    btn_fix_left->setVisible(false);

    // Arrow right & fixed modification
    btn_right = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.1, 18, 18,
                                 ":img/right.svg",
                                 ":img/right_on.svg");
    connect(btn_right, SIGNAL(clicked()), this, SLOT(shift_img_right()));

    btn_fix_right = new ToggleButton(this, widget_sd * 0.89, widget_sd * 0.1, 20, 20,
                                    ":img/fixOff.svg",
                                    ":img/fixOn.svg");
    btn_fix_right->setVisible(false);

    // Arrow up & fixed modification
    btn_up = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.17, 18, 18,
                                  ":img/up.svg",
                                  ":img/up_on.svg");
    connect(btn_up, SIGNAL(clicked()), this, SLOT(shift_img_up()));

    btn_fix_up = new ToggleButton(this, widget_sd * 0.89, widget_sd * 0.17, 20, 20,
                                     ":img/fixOff.svg",
                                     ":img/fixOn.svg");
    btn_fix_up->setVisible(false);

    // Arrow down & fixed modification
    btn_down = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.24, 18, 18,
                               ":img/down.svg",
                               ":img/down_on.svg");
    connect(btn_down, SIGNAL(clicked()), this, SLOT(shift_img_down()));

    btn_fix_down = new ToggleButton(this, widget_sd * 0.89, widget_sd * 0.24, 20, 20,
                                  ":img/fixOff.svg",
                                  ":img/fixOn.svg");
    btn_fix_down->setVisible(false);

    // blur operation
    btn_blur = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.31, 18, 18,
                                 ":img/gauss_norm.svg",
                                 ":img/gauss_norm.svg");
    connect(btn_blur, SIGNAL(clicked()), this, SLOT(blur_img()));

    btn_fix_blur = new ToggleButton(this, widget_sd * 0.89, widget_sd * 0.31, 20, 20,
                                    ":img/fixOff.svg",
                                    ":img/fixOn.svg");
    btn_fix_blur->setVisible(false);

    // blur 2x
    btn_blur2x = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.38, 18, 18,
                                 ":img/gauss_2x.svg",
                                 ":img/gauss_2x.svg");
    connect(btn_blur2x, SIGNAL(clicked()), this, SLOT(blur_img2x()));

    btn_fix_blur2x = new ToggleButton(this, widget_sd * 0.89, widget_sd * 0.38, 20, 20,
                                    ":img/fixDownOff.svg",
                                    ":img/fixDownOn.svg");
    btn_fix_blur2x->setVisible(false);

    // Clear button
    btn_clear = new PressedButton(this, widget_sd * 0.81, widget_sd * 0.7, 18, 18,
                                 ":img/clear.svg",
                                 ":img/clearOver.svg");
    connect(btn_clear, SIGNAL(clicked()), this, SLOT(clear_grid()));

    // Confirm button
    btn_confirm = new TextButton(this, widget_sd * 0.46, widget_sd * 0.82, widget_sd * 0.33, widget_sd * 0.1,
                    "RECOGNIZE");
    connect(btn_confirm, SIGNAL(clicked()), this, SLOT(recognize()));

    btn_add = new TextButton(this, widget_sd * 0.7, widget_sd * 0.82, widget_sd * 0.1, widget_sd * 0.1,
                             "+");
    connect(btn_add, SIGNAL(clicked()), this, SLOT(add_img()));
    btn_add->setVisible(false);

    mode_box = new QComboBox(this);
    mode_box->move(0, widget_sd * 0.81);
    img_labels = new QStringList({"1", "2", "3"});
    mode_box->addItems(*img_labels);
    mode_box->setVisible(false);
    connect(mode_box, SIGNAL(currentIndexChanged(int)), SLOT(img_changed(int)));
}

void DrawingArea::set_grid(int new_grid_size)
{
    grid_size = new_grid_size;
    cell_side = side_grid / grid_size;
    margin = (side_grid - (cell_side * grid_size)) / 2;
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent *e)
{
    setMouseTracking(true);
    mouseMoveEvent(e);
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e)
{
    setMouseTracking(false);
}

void DrawingArea::mouseMoveEvent(QMouseEvent *e)
{
    int x = QCursor::pos().x() - x_left_widget - ((MainWindow *) parentWidget())->x();
    int frame_height = ((MainWindow *) parentWidget())->geometry().y();
    int y = QCursor::pos().y() - y_up_widget - frame_height;

    if (x > margin + side_grid)
        return;

    int j = (x - margin) / cell_side;
    int i = (y - margin ) / cell_side;

    ((MainWindow *) parentWidget())->pick_cell(i, j);
}

void DrawingArea::clear_grid()
{

    for (int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
            ((MainWindow *) parentWidget())->set_pixel(i, j, 255);

    update();
}

void DrawingArea::set_teach_mode(int grid_sz, vector<string> *outputs)
{
    teach_mode = true;

    // Change grid configuration
    grid_size = grid_sz;
    cell_side = side_grid / grid_size;
    margin = (side_grid - (cell_side * grid_size)) / 2;

    btn_confirm->setVisible(false);
    btn_add->setVisible(true);
    mode_box->setVisible(true);
    mode_box->clear();

    btn_fix_up->setVisible(true);
    btn_fix_down->setVisible(true);
    btn_fix_left->setVisible(true);
    btn_fix_right->setVisible(true);
    btn_fix_blur->setVisible(true);
    btn_fix_blur2x->setVisible(true);

    vector<string>::iterator p = outputs->begin();
    for (int i = 0; i < outputs->size(); i++, ++p)
    {
        QString lbl = (*p).c_str();
        mode_box->addItem(lbl);
    }
}

void DrawingArea::set_recogn_mode()
{
    teach_mode = false;

    btn_confirm->setVisible(true);
    btn_add->setVisible(false);
    mode_box->setVisible(false);

    btn_fix_up->setVisible(false);
    btn_fix_down->setVisible(false);
    btn_fix_left->setVisible(false);
    btn_fix_right->setVisible(false);
    btn_fix_blur->setVisible(false);
    btn_fix_blur2x->setVisible(false);
}

void DrawingArea::save_grid()
{
    unsigned char buf[31][31];

    for (int i = 0; i < grid_size - 1; ++i)
        for (int j = 0; j < grid_size; ++j)
            grid_buf[i][j] =
            ((MainWindow *) parentWidget())->m[i][j];
}

void DrawingArea::load_grid_buf()
{
    for (int i = 0; i < grid_size - 1; ++i)
        for (int j = 0; j < grid_size; ++j)
            ((MainWindow *) parentWidget())->m[i][j] =
                    grid_buf[i][j];
}

void DrawingArea::add_img()
{
    string img_lbl = mode_box->currentText().toStdString();

    // save initial grid-matrix to the buffer
    save_grid();

    // Determine "blur intensity"
    int blur_times = 0;
    if (btn_fix_blur->is_checked())
        blur_times++;
    if (btn_fix_blur2x->is_checked())
        blur_times += 2;


    // add original image
    for (int i = 0; i < blur_times; ++i)
        blur_img();
    ((MainWindow *) parentWidget())->add_image(img_lbl);

    // check if additinal transformations is required
    if (btn_fix_up->is_checked())
    {
        load_grid_buf();
        shift_img_up();
        shift_img_up();
        for (int i = 0; i < blur_times; ++i)
            blur_img();
        ((MainWindow *) parentWidget())->add_image(img_lbl);
    }
    if (btn_fix_down->is_checked())
    {
        load_grid_buf();
        for (int i = 0; i < blur_times; ++i)
            blur_img();
        shift_img_down();
        shift_img_down();
        ((MainWindow *) parentWidget())->add_image(img_lbl);
    }
    if (btn_fix_left->is_checked())
    {
        load_grid_buf();
        for (int i = 0; i < blur_times; ++i)
            blur_img();
        shift_img_left();
        shift_img_left();
        ((MainWindow *) parentWidget())->add_image(img_lbl);
    }
    if (btn_fix_right->is_checked())
    {
        load_grid_buf();
        for (int i = 0; i < blur_times; ++i)
            blur_img();
        shift_img_right();
        shift_img_right();
        ((MainWindow *) parentWidget())->add_image(img_lbl);
    }

    clear_grid();
}

string DrawingArea::get_cur_lbl()
{
    return mode_box->currentText().toStdString();
}

void DrawingArea::img_changed(int img_index)
{
    ((MainWindow *) parentWidget())->change_image(img_index);
}

void DrawingArea::recognize()
{
    ((MainWindow *) parentWidget())->recognize_image();
}

void DrawingArea::shift_img_up()
{
    for (int i = 0; i < grid_size - 1; ++i)
        for (int j = 0; j < grid_size; ++j)
            ((MainWindow *) parentWidget())->m[i][j] =
                    ((MainWindow *) parentWidget())->m[i+1][j];
    // clear last row
    for (int j = 0; j < grid_size; j++)
        ((MainWindow *) parentWidget())->m[grid_size - 1][j] = 255;
    update();
}

void DrawingArea::shift_img_down()
{
    for (int i = grid_size - 1; i > 0; --i)
        for (int j = 0; j < grid_size; ++j)
            ((MainWindow *) parentWidget())->m[i][j] =
                    ((MainWindow *) parentWidget())->m[i-1][j];

    // clear first row
    for (int j = 0; j < grid_size; j++)
        ((MainWindow *) parentWidget())->m[0][j] = 255;
    update();
}

void DrawingArea::shift_img_left()
{
    for (int i = 0; i < grid_size; ++i)
        for (int j = 0; j < grid_size - 1; ++j)
            ((MainWindow *) parentWidget())->m[i][j] =
                    ((MainWindow *) parentWidget())->m[i][j + 1];

    // clear last column
    for (int i = 0; i < grid_size; i++)
        ((MainWindow *) parentWidget())->m[i][grid_size - 1] = 255;
    update();
}

void DrawingArea::shift_img_right()
{
    for (int i = 0; i < grid_size; ++i)
        for (int j = grid_size; j > 0; --j)
            ((MainWindow *) parentWidget())->m[i][j] =
                    ((MainWindow *) parentWidget())->m[i][j - 1];

    // clear first column
    for (int i = 0; i < grid_size; i++)
        ((MainWindow *) parentWidget())->m[i][0] = 255;
    update();
}

void DrawingArea::blur_img()
{
    MatrixXd h(3, 3);
    h <<
    0.01134374, 0.08381951, 0.01134374,
    0.08381951, 0.61934703, 0.08381951,
    0.01134374, 0.08381951, 0.01134374;

    // create matrix duplicate with m but with rims copied
    MatrixXd md(50, 50);
    // copy center
    for (int i = 0; i < grid_size; ++i)
        for (int j = 0; j < grid_size; j++)
            md(i + 1, j + 1) = ((MainWindow *) parentWidget())->m[i][j];
    // copy rims
    for (int i = 0; i < grid_size; ++i)
    {
        md(0, i + 1) = ((MainWindow *) parentWidget())->m[0][i];
        md(grid_size + 1, i + 1) = ((MainWindow *) parentWidget())->m[grid_size - 1][i];
        md(i + 1, 0) = ((MainWindow *) parentWidget())->m[i][0];
        md(i + 1, grid_size + 1) = ((MainWindow *) parentWidget())->m[i][grid_size - 1];
    }
    // diagonal elements. yes, i do not tired yet do that shit
    md(0, 0) = ((MainWindow *) parentWidget())->m[0][0];
    md(grid_size + 1, 0) = ((MainWindow *) parentWidget())->m[grid_size - 1][0];
    md(0, grid_size + 1) = ((MainWindow *) parentWidget())->m[0][grid_size - 1];
    md(grid_size + 1, grid_size + 1) = ((MainWindow *) parentWidget())->m[grid_size - 1][grid_size - 1];

    // blur
    for (int i = 1; i < grid_size + 1; ++i)
        for (int j = 1; j < grid_size + 1; ++j)
            ((MainWindow *) parentWidget())->m[i - 1][j - 1] =
                    (unsigned char) (md.block(i - 1, j - 1, 3, 3).cwiseProduct(h)).sum();

    update();
}

void DrawingArea::blur_img2x()
{
    blur_img();
    blur_img();
}

void DrawingArea::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter qp(this);

    // Background
    QPen pen(color_light_gray);
    pen.setWidth(0);
    qp.setPen(pen);
    qp.setBrush(color_light_gray);
    qp.drawRect(0, 0, side_grid, side_grid);
    pen.setColor(QColor(Qt::white));

    // Grid
    for (int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
        {
            qp.setBrush(Qt::white);

            // Change colour if necessary
            unsigned char cell_value = ((MainWindow *) parentWidget())->get_cell(i, j);
            qp.setBrush(QColor(cell_value, cell_value, cell_value));

            // Draw cell
            qp.drawRect(margin + cell_side * j, margin + cell_side * i,
                        cell_side, cell_side);
        }
}