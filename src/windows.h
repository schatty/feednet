//
// Created by Igor Kuznetsov on 30/07/16.
//

#ifndef NN_INTERFACE_WINDOWS_H
#define NN_INTERFACE_WINDOWS_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include "gui_properties.h"
#include "gui_widgets.h"


// ***************************************************
// Window
// ***************************************************

class Window : public QWidget
{
    Q_OBJECT

protected:
    int x_wnd, y_wnd;
    int w_wnd, h_wnd;

    QLabel *lbl_main;
    PressedButton *btn_close;

public:
    Window(QWidget *parent, int x, int y, int w, int h, char *name);
    virtual void open() {};

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void hide_window();
};


// ***************************************************
// Small Window
// ***************************************************

class SmallWindow : public QWidget
{
    Q_OBJECT

    int x_wnd, y_wnd;
    int w_wnd, h_wnd;

    QLabel *lbl_main;
    QLineEdit *le;
    TextButton *btn;

public:
    SmallWindow(QWidget *parent, int x, int y, int w, int h, char *text);
    void open();

protected:
    void paintEvent(QPaintEvent *e);

protected slots:
    void confirm();

};


// ***************************************************
// Window New Dataset
// ***************************************************

class WindowNewDataset : public Window
{
    Q_OBJECT

    int num_lbl_images;

    QLabel *lbl_name, *lbl_grid, *lbl_complexity, *lbl_image_plus, *lbl_desc;
    QLineEdit *le_name;
    QTextEdit *text_area;
    RoundedLabel *lbl_images[25];
    int start_lbl_x, start_lbl_y, width_lbl, height_lbl;
    Counter *counter;
    TextButton *btn_confirm;
    PressedButton *btn_plus;
    ComplexityWidget *comp_widget;
    SVGButton *text_static;

    SmallWindow *wnd_small;

public:
    WindowNewDataset(QWidget *parent1, int x, int y, int w, int h, char *name);
    void open();
    int add_img_label(QString text);

    QString get_name();
    int get_grid_size();
    int get_complexity();
    vector<string> get_outputs();
    QString get_desc();

    void reset_widget();

public slots:
    void open_label_window();
    void confirm_dataset();

    friend class SmallWindow;
};

// ***************************************************
// Window About
// ***************************************************

class WindowAbout : public Window
{
    Q_OBJECT

    QLabel *lbl_app_name;
    QLabel *lbl_app_version;
    QLabel *lbl_app_author;
    QLabel *lbl_text;
    QLabel *lbl_email;
    SVGButton *app_icon;

public:
    WindowAbout(QWidget *parent, int x, int y, int w, int h);

};

// ***************************************************
// Help Assistant Layer
// ***************************************************

class HelpAssistantLayer : public QWidget
{
    Q_OBJECT

protected:
    SVGButton *lbls[18];

public:
    HelpAssistantLayer(QWidget *parent);
    void show();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

public slots:
    void hide();

};

#endif