//
// Created by Igor Kuznetsov on 30/07/16.
//

#include <QPainter>

#include "MainWindow.h"
#include "windows.h"


// ***************************************************
// Window
// ***************************************************

Window::Window(QWidget *parent, int x, int y, int w, int h, char *name)
        : QWidget(parent)
{
    x_wnd = x;
    y_wnd = y;
    w_wnd = w;
    h_wnd = h;

    lbl_main = new QLabel(this);
    lbl_main->setFixedSize(w, h * 0.27);
    lbl_main->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lbl_main->setText(name);
    lbl_main->move(x, y);

    lbl_main->setFont(font_title_bold12);
    lbl_main->setStyleSheet(color_title_bold.c_str());

    // Close button
    btn_close = new PressedButton(this, x + w * 0.94, y + h * 0.01, w * 0.05, w * 0.05,
                                  ":img/delete.svg",
                                  ":img/delete_on.svg");

    connect(btn_close, SIGNAL(clicked()), this, SLOT(hide_window()));

    setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
}

void Window::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.setBrush(QColor(230, 230, 230, 220));
    qp.drawRect(0, 0, width(), height());

    qp.setPen(Qt::white);
    qp.setBrush(Qt::white);
    QRect rect(x_wnd, y_wnd, w_wnd, h_wnd);
    qp.drawRoundRect(rect, 3, 3);
}


void Window::hide_window()
{
    setVisible(false);
}


// ***************************************************
// Small Window
// ***************************************************

SmallWindow::SmallWindow(QWidget *parent, int x, int y, int w, int h, char *text)
        : QWidget(parent)
{
    x_wnd = x;
    y_wnd = y;
    w_wnd = w;
    h_wnd = h;

    lbl_main = new QLabel(this);
    lbl_main->setGeometry(x, y, w, h * 0.5);
    lbl_main->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lbl_main->setText(text);

    le = new QLineEdit(this);
    le->move(x + w * 0.05, y + h * 0.6 - le->height() * 0.4);
    le->setFixedWidth(w * 0.5);
    le->setFont(font11);

    lbl_main->setFont(font_title_bold11);
    lbl_main->setStyleSheet(color_title_bold.c_str());

    int w1 = w * 0.35;
    int h1 = w1 * 0.33;
    btn = new TextButton(this, le->x() + le->width() + w * 0.05, y + (h - h1) * 0.6, w1, h1, "OK");

    connect(btn, SIGNAL(clicked()), this, SLOT(confirm()));

    setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
}

void SmallWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.setBrush(QColor(230, 230, 230, 140));
    qp.drawRect(0, 0, width(), height());

    qp.setPen(Qt::white);
    qp.setBrush(Qt::white);
    QRect rect(x_wnd, y_wnd, w_wnd, h_wnd);
    qp.drawRoundRect(rect, 3, 3);
}

void SmallWindow::confirm()
{
    if (((WindowNewDataset *) parentWidget())->add_img_label(le->text()));
    {
        setVisible(false);

        // set text box of the parent widget active (!) remove this line
        // if widget is going to be independent
        ((WindowNewDataset *) parentWidget())->text_area->setFocus();
    }
}

void SmallWindow::open()
{
    le->clear();
    le->setFocus();
    setVisible(true);
}


// ***************************************************
// Window New Dataset
// ***************************************************

WindowNewDataset::WindowNewDataset(QWidget *parent1, int x, int y, int w, int h, char *name)
: Window(parent1, x, y, w, h, name)
{
    // Window Title
    lbl_main->setFixedHeight(h * 0.15);
    lbl_main->setFont(font_title_bold13);
    lbl_main->setStyleSheet(color_title_bold.c_str());

    // Name
    lbl_name = new QLabel(this);
    lbl_name->setText("NAME");
    lbl_name->move(x + w * 0.1, y + h * 0.2);
    lbl_name->setFont(font11);

    le_name = new QLineEdit(this);
    le_name->move(x + w * 0.4, y + h * 0.2 - le_name->height() * 0.1);
    le_name->setFixedWidth(w * 0.4);
    le_name->setFont(font11);

    // Grid size
    lbl_grid = new QLabel(this);
    lbl_grid->setText("GRID SIZE");
    lbl_grid->move(x + w * 0.1, y + h * 0.26);
    lbl_grid->setFont(font11);

    int cw = w * 0.22;
    counter = new Counter(this, x + w * 0.48, y + h * 0.26, cw, cw / 4, 28, 25, 30, 11);

    // Complexity
    lbl_complexity = new QLabel(this);
    lbl_complexity->setText("COMPLEXITY");
    lbl_complexity->move(x + w * 0.1, y + h * 0.32);
    lbl_complexity->setFont(font11);

    cw = w * 0.29;
    comp_widget = new ComplexityWidget(this, x + w * 0.44, y + h * 0.32, cw / 7, 7);

    // Image labels
    num_lbl_images = 0;

    lbl_image_plus = new QLabel(this);
    lbl_image_plus->setText("IMAGE LABELS");
    lbl_image_plus->move(x + w * 0.1, y + h * 0.38);
    lbl_image_plus->setFont(font11);

    int x1 = lbl_image_plus->x() + lbl_image_plus->width();
    int y1 = lbl_image_plus->y();// - lbl_image_plus->height() * 0.1;

    btn_plus = new PressedButton(this, x1, y1, w * 0.05, w * 0.05,
                                 ":img/plus_sign.svg",
                                 ":img/plus_sign_over.svg");
    connect(btn_plus, SIGNAL(clicked()), this, SLOT(open_label_window()));

    start_lbl_x = x + w * 0.1;
    start_lbl_y = y + h * 0.43;
    width_lbl = w * 0.15;
    height_lbl = h * 0.04;
    for (int i = 0; i < 20; i++)
        lbl_images[i] = new RoundedLabel(this, width_lbl, height_lbl);

    // Description
    lbl_desc = new QLabel(this);
    lbl_desc->setText("DESCRIPTION");
    lbl_desc->move(x + w * 0.1, y + h * 0.62);
    lbl_desc->setFont(font11);

    text_area = new QTextEdit(this);
    text_area->move(x + w * 0.1, y + h * 0.67);
    text_area->setFixedSize(w * 0.8, h * 0.2);
    text_area->setFont(font11);

    // Confirm button
    int btn_w = width() * 0.07;
    btn_confirm = new TextButton(this, x + (w - btn_w) / 2, y + h * 0.9, btn_w, btn_w * 0.3, "CONFIRM");
    connect(btn_confirm, SIGNAL(clicked()), this, SLOT(confirm_dataset()));

    int w_tmp = w * 0.6;
    text_static = new SVGButton(this, x + w * 0.25, y + h * 0.4, w_tmp, w_tmp * 0.35,
                                ":img/text1.svg");

    // Label small window
    int w1 = w * 0.6;
    int h1 = height() * 0.15;
    x1 = (width() - w1) / 2;
    y1 = (height() - h1) / 2;
    wnd_small = new SmallWindow(this, x1, y1, w1, h1, "IMAGE LABEL:");
    wnd_small->setVisible(false);
}

void WindowNewDataset::open_label_window()
{
    wnd_small->open();
}

void WindowNewDataset::open()
{
    setVisible(true);
    reset_widget();
    le_name->setFocus();
}

int WindowNewDataset::add_img_label(QString text)
{
    // Check for non-emptiness and non-identity
    if (text.isEmpty())
        return 0;
    for (int i = 0; i < num_lbl_images; ++i)
        if (! text.toStdString().compare(lbl_images[i]->get_text()))
            return 0;

    text_static->setVisible(false);

    int x_pos = start_lbl_x + (width_lbl + w_wnd / 50) * (num_lbl_images % 5);
    int y_pos = start_lbl_y + (height_lbl + y_wnd / 40) * (num_lbl_images / 5);
    lbl_images[num_lbl_images]->set_label(x_pos, y_pos, text, 11);

    num_lbl_images++;
    if (num_lbl_images == 8)
        btn_plus->setVisible(false);

    return 1;
}

QString WindowNewDataset::get_name()
{
    return le_name->text();
}

int WindowNewDataset::get_grid_size()
{
    return counter->get_value();
}

int WindowNewDataset::get_complexity()
{
    return comp_widget->get_complexity() + 1;
}

vector<string> WindowNewDataset::get_outputs()
{
    vector<string> outputs;
    for (int i = 0; i < num_lbl_images; ++i)
        outputs.push_back(lbl_images[i]->get_text());
    return outputs;
}

QString WindowNewDataset::get_desc()
{
    return text_area->toPlainText();
}

void WindowNewDataset::confirm_dataset()
{
    hide_window();
    ((MainWindow *) parentWidget())->create_dataset();
}

void WindowNewDataset::reset_widget()
{
    le_name->clear();
    text_area->clear();
    for (int i = 0; i < 20; ++i)
        lbl_images[i]->setVisible(false);
    text_static->setVisible(true);
    num_lbl_images = 0;
}


// ***************************************************
// Window About
// ***************************************************

WindowAbout::WindowAbout(QWidget *parent, int x, int y, int w, int h)
: Window(parent, x, y, w, h, "")
{
    int icon_side = w * 0.2;
    app_icon = new SVGButton(this, x + w / 2 - icon_side / 2, y + h * 0.15, icon_side, icon_side,
                                ":img/icon.svg");

    QFont font("Source Sans Pro", 13);
    font.setBold(true);
    lbl_app_name = new QLabel("Feednet", this);
    lbl_app_name->move(x + w * 0.43, y + h * 0.38);
    lbl_app_name->setFont(font);
    lbl_app_name->setStyleSheet(color_title_bold.c_str());

    font = QFont("Source Sans Pro", 12);
    lbl_app_version = new QLabel("Version 1.0.0 (Sep 2016)", this);
    lbl_app_version->move(x + w * 0.32, y + h * 0.44);
    lbl_app_version->setFont(font);
    lbl_app_version->setStyleSheet(color_title_bold.c_str());

    lbl_app_author = new QLabel("Author: Igor Kuznetsov", this);
    lbl_app_author->move(x + w * 0.1, y + h * 0.57);
    lbl_app_author->setFont(font);
    lbl_app_author->setStyleSheet(color_title_bold.c_str());

    lbl_text = new QLabel("I would be happy to get your feedback about this \nsmall project. Please feel free to contact me :)", this);
    lbl_text->setFixedSize(w * 0.8, h * 0.25);
    lbl_text->move(x + w * 0.1, y + h * 0.56);
    lbl_text->setFont(font);
    lbl_text->setStyleSheet(color_title_bold.c_str());

    lbl_email = new QLabel("igorkuznetsov14@gmail.com", this);
    lbl_email->move(x + w * 0.28, y + h * 0.8);
    lbl_email->setFont(font);
    lbl_email->setStyleSheet(color_title_bold.c_str());
    lbl_email->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

// ***************************************************
// Help Assistant Layer
// ***************************************************

HelpAssistantLayer::HelpAssistantLayer(QWidget *parent)
: QWidget(parent)
{
    int w = parentWidget()->width();
    int h = parentWidget()->height();
    setGeometry(0, 0, w, h);

    lbls[0] = new SVGButton(this, w * 0.1, h * 0.0, w * 0.15, w * 0.15, ":img/text_help1.svg");
    lbls[1] = new SVGButton(this, w * 0.14, h * 0.07, w * 0.15, w * 0.15, ":img/text_help2.svg");
    lbls[2] = new SVGButton(this, w * 0.1, h * 0.14, w * 0.15, w * 0.15, ":img/text_help3.svg");
    lbls[3] = new SVGButton(this, w * 0.1, h * 0.23, w * 0.15, w * 0.15, ":img/text_help4.svg");
    lbls[4] = new SVGButton(this, w * 0.005, h * 0.325, w * 0.15, w * 0.15, ":img/text_help5.svg");
    lbls[5] = new SVGButton(this, w * 0.24, -h * 0.08, w * 0.15, w * 0.15, ":img/text_help6.svg");
    lbls[6] = new SVGButton(this, w * 0.3, h * 0.0, w * 0.15, w * 0.15, ":img/text_help7.svg");
    lbls[7] = new SVGButton(this, w * 0.3, h * 0.18, w * 0.15, w * 0.15, ":img/text_help8.svg");
    lbls[8] = new SVGButton(this, w * 0.03, h * 0.6, w * 0.15, w * 0.15, ":img/text_help9.svg");
    lbls[9] = new SVGButton(this, w * 0.265, h * 0.67, w * 0.25, w * 0.25 * 0.3, ":img/text_help10.svg");
    lbls[10] = new SVGButton(this, w * 0.395, h * 0.28, w * 0.15, w * 0.15, ":img/text_help11.svg");
    lbls[11] = new SVGButton(this, w * 0.54, h * 0.025, w * 0.15, w * 0.15, ":img/text_help12.svg");
    lbls[12] = new SVGButton(this, w * 0.545, h * 0.075, w * 0.15, w * 0.15, ":img/text_help13.svg");
    lbls[13] = new SVGButton(this, w * 0.69, h * 0.2, w * 0.15, w * 0.15, ":img/text_help14.svg");
    lbls[14] = new SVGButton(this, w * 0.81, h * 0.07, w * 0.15, w * 0.15, ":img/text_help15.svg");
    lbls[15] = new SVGButton(this, w * 0.81, h * 0.09, w * 0.15, w * 0.15, ":img/text_help16.svg");
    lbls[16] = new SVGButton(this, w * 0.7, h * 0.75, w * 0.15, w * 0.15, ":img/text_help17.svg");
    lbls[17] = new SVGButton(this, w * 0.36, h * 0.11, w * 0.15, w * 0.15, ":img/text_help18.svg");

    connect(lbls[0], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[1], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[2], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[3], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[4], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[5], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[6], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[7], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[8], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[9], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[10], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[11], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[12], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[13], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[14], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[15], SIGNAL(clicked()), this, SLOT(hide()));
    connect(lbls[16], SIGNAL(clicked()), this, SLOT(hide()));

    setVisible(false);
}

void HelpAssistantLayer::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.setBrush(QColor(230, 230, 230, 180));
    qp.drawRect(0, 0, width(), height());
}

void HelpAssistantLayer::mousePressEvent(QMouseEvent *e)
{
    hide();
}

void HelpAssistantLayer::hide()
{
    setVisible(false);
}

void HelpAssistantLayer::show()
{
    // Dataset extendible chekcboxes comment
    if (((MainWindow *) parentWidget())->ds_widget->is_teach_mode())
    {
        lbls[11]->setVisible(false);
        lbls[12]->setVisible(false);
        lbls[17]->setVisible(true);
    }
    else
    {
        lbls[11]->setVisible(true);
        lbls[12]->setVisible(true);
        lbls[17]->setVisible(false);
    }

    // VisualNet comments
    if (((MainWindow *) parentWidget())->visual_net->isVisible())
    {
        lbls[13]->setVisible(true);
        lbls[14]->setVisible(true);
        lbls[15]->setVisible(true);
        lbls[16]->setVisible(true);
    } else
    {
        lbls[13]->setVisible(false);
        lbls[14]->setVisible(false);
        lbls[15]->setVisible(false);
        lbls[16]->setVisible(false);
    }

    setVisible(true);
}