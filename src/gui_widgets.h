//
// Created by Igor Kuznetsov on 28/07/16.
//

#ifndef NN_INTERFACE_GUI_WIDGETS_H
#define NN_INTERFACE_GUI_WIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <qsvgwidget.h>
#include <qsvgrenderer.h>
#include <string>
#include <QPushButton>
#include <QCheckBox>
#include <iostream>

using namespace std;

// ***************************************************
// Line Edit
// ***************************************************

class LineEdit : public QLineEdit
{
    QLabel *lbl;

public:
    LineEdit(QWidget *parent, int x, int y, char *lbl_name, char *line_text, int font_size);
    double get_num();
};


// ***************************************************
// SVGButton
// ***************************************************

class SVGButton : public QPushButton
{
    Q_OBJECT

public:
    SVGButton(QWidget *parent, int x, int y, int w, int h, string icon_path);
    void load_img(string path);

protected:
    QString path;
    QSvgRenderer *svg_rnd;

    void paintEvent(QPaintEvent *e);
};


// ***************************************************
// PressButton
// ***************************************************

class PressedButton : public SVGButton
{
    Q_OBJECT

    QString path_pressed;

public:
    PressedButton(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};


// ***************************************************
// Toggle Button
// ***************************************************

class ToggleButton : public SVGButton
{

protected:
    bool state_on;
    QString path_on;

public:
    ToggleButton(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed);
    void turn_on();
    void turn_off();
    bool is_checked();

protected:
    void mousePressEvent(QMouseEvent *e);
};


// ***************************************************
// Text Button
// ***************************************************

class TextButton : public QPushButton
{
    QColor color_background;
    QColor color_off;
    QLabel *lbl;

public:
    TextButton(QWidget *parent, int x, int y, int w, int h, char *text, string bck_color="#e6e6e6");
    void set_text(QString str);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
};


// ***************************************************
// Counter
// ***************************************************

class Counter : QWidget
{
Q_OBJECT

    int val;
    int upper, lower;
    QLabel *lbl;
    PressedButton *btn_plus;
    PressedButton *btn_minus;

public:
    Counter(QWidget *parent, int x, int y, int w, int h, int init_value,
            int lower_val, int upper_val, int font_size=11);
    int get_value();

private slots:
    void increase();
    void decrease();

};


// ***************************************************
// Complexity widget
// ***************************************************

class Star : public ToggleButton
{
    Q_OBJECT

public:
    Star(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed,
            int star_id);

protected:
    int id;
    void mousePressEvent(QMouseEvent *e);
};

class ComplexityWidget : QWidget
{
    Q_OBJECT

    int max_compl;
    int cur_compl;
    Star *stars[10];

public:
    ComplexityWidget(QWidget *parent, int x, int y, int h, int num_stars);
    void set_complexity(int complexity);
    int get_complexity();

private slots:

};


// ***************************************************
// Rounded Label
// ***************************************************

class RoundedLabel : public QWidget
{
    Q_OBJECT

    QLabel *lbl;

public:
    RoundedLabel(QWidget *parent, int w, int h);
    void set_label(int x, int y, QString text, int font_size=11);
    string get_text();

protected:
    void paintEvent(QPaintEvent *e);
};


// ***************************************************
// Tool Widget
// ***************************************************

class ToolBtn : public Star
{
public:
    ToolBtn(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed,
            int star_id);
protected:
    void mousePressEvent(QMouseEvent *e);
};


class ToolWidget : public QWidget
{
    Q_OBJECT

    ToggleButton *tools [6];
    int cur_tool_id;

public:
    ToolWidget(QWidget *parent, int x, int y, int w, int h);
    void set_tool(int tool_id);

protected:
    void paintEvent(QPaintEvent *e);
};

// ***************************************************
// CheckBox
// ***************************************************

class CheckBox : public QPushButton
{
    Q_OBJECT

    SVGButton *btn_svg;
    QLabel *lbl;
    int id;
    bool is_on;
    bool alone;  // flag = true if CheckBox is independent from ohter checkboxes

public:
    CheckBox(QWidget *parent, int cb_id, int x, int y, int w, int h, string name,
             bool checked=false, bool cb_alone=false);

    bool is_checked();
    void turn_off();
    void turn_on();

    void paintEvent(QPaintEvent *e);

public slots:
    void click();
};

// ***************************************************
// CheckBoxesWidget
// ***************************************************

class CheckBoxesWidget : public QWidget
{
    Q_OBJECT
    CheckBox *checkboxes [4];
    QLabel *lbl_title;

    vector<string> cb_names;
    int cur_ind;

public:
    CheckBoxesWidget(QWidget *parent, int x, int y, int w, int h, string title, vector<string> names);
    int get_checked_index();

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void update_widget(int id_checked);
};

#endif