//
// Created by Igor Kuznetsov on 02/08/16.
//

#ifndef NN_INTERFACE_DSPREVIEWWIDGET_H
#define NN_INTERFACE_DSPREVIEWWIDGET_H

#include "gui_widgets.h"
#include "data_api.h"

#include <QWidget>
#include <QLabel>
#include <map>

class PageLabel : public TextButton
{
    int id;

public:
    PageLabel(QWidget *parent, int x, int y, int w, int h, char *text, int btn_id);

protected:
    void mousePressEvent(QMouseEvent *e);
};

class PagePreview : public QWidget
{
Q_OBJECT

    bool page_on;
    int img_size;
    string img_name;

    // Graphic vars
    int mrg_x, mrg_y;
    int num_img_per_line;
    int cur_line;
    int space_between;

public:
    PagePreview(QWidget *parent, int x, int y, int w, int h);
    bool is_on() { return page_on; }
    void set_img_name(string img_nm);
    void draw_image(QPainter *qp, vector<unsigned char> v, int x, int y);
    void set_img_size(int size);

protected:
    void paintEvent(QPaintEvent *e);

public slots:

};

class DSPreviewWidget : public QWidget
{
    Q_OBJECT

    bool data_initialized;

    int num_pages;
    QLabel *lbls_all_tab [11];
    PageLabel *btn_lbl_page [11];
    PagePreview *pages [11];
    vector<string> img_names;

public:
    DSPreviewWidget(QWidget *parent, int x, int y, int w, int h);
    void init_widget(vector<string> outputs, multimap<string, vector<unsigned char>> *d1, int grid_size);
    void set_page(int page_id);
    bool data_is_ready();

    multimap<string, vector<unsigned char>> *data;

protected:
    //void paintEvent(QPaintEvent *e);

public slots:

    friend class PagePreview;

};

#endif //NN_INTERFACE_DSPREVIEWWIDGET_H
