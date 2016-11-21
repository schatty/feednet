//
// Created by Igor Kuznetsov on 29/07/16.
//

#include <QPainter>

#include "dataset_widget.h"
#include "MainWindow.h"
#include "gui_properties.h"

using namespace std;

dataset_widget::dataset_widget(QWidget *parent, int x, int y, int width, int height)
: QWidget(parent)
{
    teach_mode = false;
    num_images = 0;
    total_num_img = 0;
    for (int i = 0; i < 30; ++i)
        num_by_type[i] = 0;
    next_limit = 50;

    lbl_title = new QLabel("Dataset", this);
    lbl_title->setFixedWidth(width);
    lbl_title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    lbl_title->setStyleSheet(color_title_bold.c_str());
    lbl_title->setFont(font_title_bold13);

    mrg_title = height * 0.07;

    lbl_name = new QLabel("TIC-TAC-TOE", this);
    lbl_name->setGeometry(width * 0.05, 0, width, mrg_title + height * 0.2);
    lbl_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lbl_name->setFont(font13);

    lbl_size = new QLabel("100 IMG", this);
    lbl_size->setGeometry(width * 0.05, 0, width, mrg_title + height * 0.3);
    lbl_size->setFont(font10);

    lbl_desc = new QLabel("this is a description label with description", this);
    lbl_desc->setGeometry(width * 0.05, height * 0.4, width * 0.9, height * 0.6);
    lbl_desc->setFont(font12);
    lbl_desc->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lbl_desc->setWordWrap(true);

    int star_side = width * 0.05;
    for (int i = 0; i < 7; i++)
    {
        stars[i] = new SVGButton(this, x + width * 0.75 + i * star_side, mrg_title + height * 0.2, star_side, star_side,
                                 ":img/star.svg");
    }

    for (int i = 0; i < 10; i++)
    {
        lbl_img_names[i] = new QLabel("triangle", this);
        lbl_img_names[i]->setGeometry(width * 0.05, mrg_title + height * 0.22 + height * 0.075 * i, width * 0.3, height * 0.05);
        lbl_img_names[i]->setFont(font11);

        lbl_img_size[i] = new QLabel("150", this);
        lbl_img_size[i]->setGeometry(width * 0.83, mrg_title + height * 0.22 + height * 0.075 * i, width * 0.3, height * 0.05);
        lbl_img_size[i]->setFont(font11);
    }

    int btn_w = width * 0.35;
    int btn_h = btn_w * 0.28;

    btn_new = new TextButton(this, width * 0.61, mrg_title + height * 0.04, btn_w, btn_h, "+ DATASET");
    connect(btn_new, SIGNAL(clicked()), this, SLOT(add_new_dataset()));

    btn_load = new TextButton(this, width * 0.61, mrg_title + height * 0.13, btn_w, btn_h, "LOAD");
    connect(btn_load, SIGNAL(clicked()), this, SLOT(load_ds()));

    btn_confirm = new TextButton(this, width * 0.61, mrg_title + height * 0.04, btn_w, btn_h, "CONFIRM");
    connect(btn_confirm, SIGNAL(clicked()), this, SLOT(confirm_dataset()));
    btn_confirm->setVisible(false);

    setGeometry(x, y, width, height);
    set_recogn_mode();
}

void dataset_widget::init_widget(string name, int size, int complexity, string desc)
{
    QString q_name = name.c_str();
    lbl_name->setText(q_name.toUpper());
    lbl_size->setText((to_string(size) + " IMG").c_str());
    lbl_desc->setText(desc.c_str());

    int i;
    for (i = 0; i < complexity; ++i)
        stars[i]->load_img(":img/star_on.svg");
    for (; i < 7; i++)
        stars[i]->load_img(":img/star.svg");
}

void dataset_widget::set_teaching_mode(QString name, vector<string> outputs)
{
    int i;
    teach_mode = true;
    num_images = outputs.size();

    btn_new->setVisible(false);
    btn_load->setVisible(false);
    btn_confirm->setVisible(true);

    lbl_desc->setVisible(false);
    for (int i = 0; i < 7; i++)
        stars[i]->setVisible(false);

    // Set name
    lbl_name->setText(name.toUpper());
    lbl_size->setText("0 IMG");

    for (i = 0; i < num_images; i++)
    {
        lbl_img_names[i]->setVisible(true);
        QString lbl_text = outputs[i].c_str();
        lbl_img_names[i]->setText(lbl_text);

        lbl_img_size[i]->setVisible(true);
        lbl_img_size[i]->setText("0");
    }
    for (; i < 10; ++i)
    {
        lbl_img_names[i]->setVisible(false);
        lbl_img_size[i]->setVisible(false);
    }
    update();
}

void dataset_widget::set_recogn_mode()
{
    teach_mode = false;

    btn_new->setVisible(true);
    btn_load->setVisible(true);
    btn_confirm->setVisible(false);

    lbl_desc->setVisible(true);
    for (int i = 0; i < 7; i++)
        stars[i]->setVisible(true);

    for (int i = 0; i < 10; i++)
    {
        lbl_img_names[i]->setVisible(false);
        lbl_img_size[i]->setVisible(false);
    }

    // Update description
    lbl_desc->setText(((MainWindow *) parentWidget())->ds->comment.c_str());

    update();
}

void dataset_widget::add_new_dataset()
{
    ((MainWindow *) parentWidget())->open_new_dataset_window();
}

void dataset_widget::add_img(string img_name)
{
    int i;
    for (i = 0; i < num_images; i++)
        if (! img_name.compare(lbl_img_names[i]->text().toStdString()))
            break;

    ++num_by_type[i];
    string str = to_string(lbl_img_size[i]->text().toInt() + 1);
    lbl_img_size[i]->setText(str.c_str());

    total_num_img++;
    QString qstr = to_string(total_num_img).c_str();
    lbl_size->setText(qstr + QString(" IMG"));

    update();
}

void dataset_widget::confirm_dataset()
{
    ((MainWindow *) parentWidget())->save_dataset();
}

void dataset_widget::load_ds()
{
    ((MainWindow *) parentWidget())->open_dataset_house_window();
}

void dataset_widget::reset_widget()
{
    for (int i = 0; i < 20; ++i)
        num_by_type[i] = 0;
    next_limit = 50;
}

void dataset_widget::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);

    // Progress
    if (teach_mode)
    {
        // Background
        paint_background(qp, 0, mrg_title, width(), height() * 0.25 + height() * 0.75  / 10 * num_images);

        for (int i = 0; i < num_images; i++)
        {
            // draw gray background
            qp.setPen(QColor(220, 220, 220));
            qp.setBrush(QColor(220, 220, 220));
            qp.drawRoundedRect(width() * 0.28, mrg_title + height() * 0.22 + height() * 0.075 * i,
                               width() * 0.5, height() * 0.05, 3, 3);
            // fulfill area with some amount of green
            qp.setPen(color_green);
            qp.setBrush(color_green);

            double percent = num_by_type[i] > next_limit ? 1 : (num_by_type[i] / (float) next_limit);
            if (num_by_type[i] > next_limit)
                next_limit *= 1.5;

            qp.drawRoundedRect(width() * 0.28, mrg_title + height() * 0.22 + height() * 0.075 * i,
                               width() * 0.5 * percent, height() * 0.05, 2, 2);
        }
    }
    else
    {
        // Background
        // |?| ... why "* 2" at the end? I don't understand it ...
        paint_background(qp, 0, mrg_title, width(), height() - mrg_title * 2);
    }
}