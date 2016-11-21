//
// Created by Igor Kuznetsov on 28/07/16.
//

#include <QPainter>
#include "CharactPanel.h"
#include "MainWindow.h"
#include "gui_properties.h"

CharactPanel::CharactPanel(QWidget *parent, int x, int y, int width, int height) : QWidget(parent)
{
    setGeometry(x, y, width, height);

    lbl_title = new QLabel("HYPER PARAMETERS", this);
    lbl_title->setFixedWidth(width);
    lbl_title->move(0, height * 0.03);
    lbl_title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    lbl_title->setStyleSheet(color_title_bold.c_str());
    lbl_title->setFont(font_title_bold11);

    int mrg_y = height * 0.15;
    int v_step = width * 0.17;

    eras = new LineEdit(this, width * 0.05, mrg_y, "NO EPOCHS: ", "30", 10);
    eras->setFixedWidth(width * 0.45);

    no_exm = new LineEdit(this, width * 0.05, mrg_y + v_step * 1, "BATCH SIZE: ", "20", 10);
    no_exm->setFixedWidth(width * 0.45);

    lrn_rate = new LineEdit(this, width * 0.05, mrg_y + v_step * 2, "LRN RATE:", "3", 10);
    lrn_rate->setFixedWidth(width * 0.45);


    lbl_test_part = new QLabel(this);
    lbl_test_part->setText("TEST PART:   17%");
    lbl_test_part->move(width * 0.05, height * 0.5);
    lbl_test_part->setFont(font_common);

    sldr_test_part = new QSlider(this);
    sldr_test_part->move(width * 0.05, height * 0.55);
    sldr_test_part->setFixedWidth(width * 0.9);
    sldr_test_part->setOrientation(Qt::Horizontal);
    sldr_test_part->setValue(17);
    connect(sldr_test_part, SIGNAL(valueChanged(int)), this, SLOT(change_test_part(int)));

    cb_calc_acc = new CheckBox(this, 0, width * 0.05, mrg_y + height * 0.57, width, width * 0.1,
                               "WATCH PROGRESS", true, true);

    int btn_w = width * 0.49;
    btn_teach = new TextButton(this, (width - btn_w) /2, height * 0.87, btn_w, btn_w * 0.3, "TRAIN");
    connect(btn_teach, SIGNAL(clicked()), this, SLOT(train()));
}

void CharactPanel::change_test_part(int val)
{
    string str = "TEST PART:   " + to_string(val) + "%";
    lbl_test_part->setText(str.c_str());
}

void CharactPanel::train()
{
    ((MainWindow *) parentWidget())->train_network();
}

void CharactPanel::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    paint_background(qp, 0, 0, width(), height());
}

// GETTERS

int CharactPanel::get_epochs()
{
    int epochs = eras->text().toInt();
    if (!epochs)
    {
        ((MainWindow *) parentWidget())->set_message("Incorrent number of epochs", 2);
        return -1;
    }
    return epochs;
}

int CharactPanel::get_mini_batch_size()
{
    int batch = no_exm->text().toInt();
    if (!batch)
    {
        ((MainWindow *) parentWidget())->set_message("Incorrent batch size", 2);
        return -1;
    }
    return batch;
}

double CharactPanel::get_learn_rate()
{
    double mu = lrn_rate->text().toDouble();
    if (!mu)
    {
        ((MainWindow *) parentWidget())->set_message("Incorrent learning rate", 2);
        return -1;
    }
    return mu;
}

double CharactPanel::get_test_rate()
{
    return sldr_test_part->value() / 100.0;
}

bool CharactPanel::get_watch_process()
{
    return cb_calc_acc->is_checked();
}