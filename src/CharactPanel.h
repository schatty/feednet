//
// Created by Igor Kuznetsov on 28/07/16.
//

#ifndef NN_INTERFACE_CHARACTPANEL_H
#define NN_INTERFACE_CHARACTPANEL_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSlider>

#include "gui_widgets.h"
#include "gui_properties.h"

class CharactPanel : public QWidget
{
    Q_OBJECT

    QLabel *lbl_title;

    LineEdit *eras, *no_exm, *lrn_rate;
    QLabel *lbl_test_part;
    QSlider *sldr_test_part;
    CheckBox *cb_calc_acc;
    QLineEdit *le1, *le2;
    TextButton *btn_teach;

public:
    CharactPanel(QWidget *parent, int x, int y, int width, int height);

    // getters
    int get_epochs();
    int get_mini_batch_size();
    double get_learn_rate();
    double get_test_rate();
    bool get_watch_process();

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void change_test_part(int val);
    void train();
};

#endif //NN_INTERFACE_CHARACTPANEL_H
