//
// Created by Igor Kuznetsov on 07/08/16.
//

#ifndef NN_INTERFACE_DATASETHOUSE_H
#define NN_INTERFACE_DATASETHOUSE_H

#include <QWidget>

#include "gui_widgets.h"

class Icon : public QWidget
{
    string name;
    int id;
    SVGButton *svg_icon;
    QLabel *lbl_name;

public:
    Icon(QWidget *parent);
    void set_icon(int x, int y, int w, int h, int id_icon, string name, int num_img_types);

protected:
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
};

struct ds_preview_info
{
    int id;
    string name;
    string path;
    int num_img_types;
    int grid_size;
    int complexity;
    vector<string> outputs;
    string desc;

    ds_preview_info(int ds_id, string nm, string ds_path, int num_types, int grid_sz, int complex, vector<string> outp, string descr)
    {
        id = ds_id;
        path = ds_path;
        name = nm;
        num_img_types = num_types;
        grid_size = grid_sz;
        complexity = complex;
        outputs = outp;
        desc = descr;
    }
};


class DatasetHouse : public QWidget
{
    Q_OBJECT

    // Grahical stuff
    int wnd_x, wnd_y, wnd_w, wnd_h;
    int mrg_x, mrg_y;
    int icon_side;
    int icon_width, icon_height;

    QLabel *lbl_title;
    SVGButton *svg_icon;
    TextButton *btn_confirm;
    SVGButton *btn_close;
    string wrk_dir_path;

    Icon *icons [50];
    vector<ds_preview_info> ds_info;
    int id_selected_ds;
    int ds_id_count;
    int num_internal_datasets;

    // Preview widgets
    QLabel *lbl_ds_name;
    QLabel *lbl_ds_size;
    QLabel *lbl_outputs;
    QLabel *lbl_desc, *lbl_desc_text;
    QLabel *lbl_grid_size;
    SVGButton *stars [7];

    // Buttons
    TextButton *btn_set_wrk_dir;
    TextButton *btn_open_file;

public:
    DatasetHouse(QWidget *parent, int x, int y, int w, int h);
    int process_ds_file(int id, string path);
    void show_ds_info(int ds_id);
    void set_visible_preview_widets(bool visbl);

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void confirm_dataset();
    void hide_window();
    void open_file();
    void set_wrk_dir();

    friend class Icon;
};


#endif //NN_INTERFACE_DATASETHOUSE_H
