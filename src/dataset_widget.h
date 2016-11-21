//
// Created by Igor Kuznetsov on 29/07/16.
//

#ifndef NN_INTERFACE_DATASET_WIDGET_H
#define NN_INTERFACE_DATASET_WIDGET_H

#import <QWidget>
#import <QLabel>
#import <QPushButton>

#import "gui_widgets.h"

class dataset_widget : QWidget
{
    Q_OBJECT

    bool teach_mode;
    int num_images;
    int total_num_img;
    int mrg_title;
    int num_by_type[20];
    int next_limit;

    QLabel *lbl_title;
    QLabel *lbl_name, *lbl_size;
    TextButton *btn_new, *btn_load, *btn_confirm;
    QLabel *lbl_img_names [15], *lbl_img_size [15];
    QLabel *lbl_desc;
    SVGButton *stars [7];

public:
    dataset_widget(QWidget *parent, int x, int y, int width, int height);
    void init_widget(string name, int size, int complexity, string desc);
    void add_img(string img_name);

    void reset_widget();

    // getters
    bool is_teach_mode() { return teach_mode; }

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void set_teaching_mode(QString name, vector<string> outpus);
    void set_recogn_mode();
    void add_new_dataset();
    void confirm_dataset();
    void load_ds();
};


#endif //NN_INTERFACE_DATASET_WIDGET_H
