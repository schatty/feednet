//
// Created by Igor Kuznetsov on 28/07/16.
//

#include <QPainter>

#include "gui_widgets.h"
#include "gui_properties.h"
//#include "gui_properties.h"
#include "MainWindow.h"
#include <QSvgWidget>
#include <QSvgRenderer>
#include <QPixmap>

#include <iostream>
#include <QPushButton>

using namespace std;

// ***************************************************
// LineEdit
// ***************************************************

LineEdit::LineEdit(QWidget *parent, int x, int y, char *lbl_name, char *line_text, int font_size)
        : QLineEdit(parent)
{
    setFont(QFont("Source Sans Pro", font_size));
    setText(line_text);

    lbl = new QLabel(lbl_name, parent);
    lbl->setFont(QFont("Source Sans Pro", font_size + 1));
    lbl->move(x, y);

    move(x + width() * 0.75, y - height() * 0.1);
}

double LineEdit::get_num()
{
    return text().toDouble();
}

// ***************************************************
// SVGButton
// ***************************************************

SVGButton::SVGButton(QWidget *parent, int x, int y, int w, int h, string icon_path)
        : QPushButton(parent)
{
    path = icon_path.c_str();
    svg_rnd = new QSvgRenderer(QString(path));

    setGeometry(x, y, w, h);
}

void SVGButton::load_img(string path)
{
    QString q_str = path.c_str();
    svg_rnd->load(q_str);
}

void SVGButton::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    svg_rnd->render(&qp);
}


// ***************************************************
// PressedButton
// ***************************************************

PressedButton::PressedButton(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed)
: SVGButton(parent1, x, y, w, h, icon_path)
{
    path_pressed = icon_path_pressed.c_str();
}

void PressedButton::mousePressEvent(QMouseEvent *e)
{
    SVGButton::mousePressEvent(e);
    svg_rnd->load(path_pressed);
    update();
}

void PressedButton::mouseReleaseEvent(QMouseEvent *e)
{
    SVGButton::mouseReleaseEvent(e);
    svg_rnd->load(path);
    update();
}


// ***************************************************
// ToggleButton
// ***************************************************

ToggleButton::ToggleButton(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed)
        : SVGButton(parent1, x, y, w, h, icon_path)
{
    path_on = icon_path_pressed.c_str();
    state_on = false;
}

void ToggleButton::mousePressEvent(QMouseEvent *e)
{
    if (state_on)
        svg_rnd->load(path);
    else
        svg_rnd->load(path_on);
    state_on = !state_on;

    update();
}

void ToggleButton::turn_on()
{
    svg_rnd->load(path_on);
    state_on = true;
    update();
}

void ToggleButton::turn_off()
{
    svg_rnd->load(path);
    state_on = false;
    update();
}

bool ToggleButton::is_checked()
{
    return state_on;
}


// ***************************************************
// TextButton
// ***************************************************

TextButton::TextButton(QWidget *parent, int x, int y, int w, int h, char *text,
                        string bck_color)
: QPushButton(parent)
{
    lbl = new QLabel(this);
    lbl->setText(text);
    lbl->setGeometry(0, 0, w, h);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbl->setStyleSheet("color: #58595B");
    lbl->setFont(font11);

    color_off = QColor(bck_color.c_str());
    color_background = QColor(bck_color.c_str());

    setGeometry(x, y, w, h);
}

void TextButton::set_text(QString str)
{
    lbl->setText(str);
}

void TextButton::mousePressEvent(QMouseEvent *e)
{
    QPushButton::mousePressEvent(e);
    color_background = color_btn_on;
    update();
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    QPushButton::mouseReleaseEvent(e);
    color_background = color_off;
    update();
}

void TextButton::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.setBrush(color_background);
    qp.setPen(color_background);
    QRect rect(0, 0, width(), height());
    qp.drawRoundedRect(rect, 3, 3);
}


// ***************************************************
// Counter
// ***************************************************

Counter::Counter(QWidget *parent, int x, int y, int w, int h, int init_value,
                int lower_val, int upper_val, int font_size)
: QWidget(parent)
{
    val = init_value;
    upper = upper_val;
    lower = lower_val;

    lbl = new QLabel(this);
    lbl->setText(to_string(val).c_str());
    lbl->setGeometry(w * 0.25, 0, w * 0.5, h);
    lbl->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    lbl->setFont(QFont("Source Sans Pro", font_size));

    btn_plus = new PressedButton(this, w * 0.75, 0, h, h, ":img/plus1.svg",
                                 ":img/plus1_over.svg");
    connect(btn_plus, SIGNAL(clicked()), this, SLOT(increase()));

    btn_minus = new PressedButton(this, 0, 0, h, h, ":img/minus1.svg",
                                 ":img/minus1_over.svg");
    connect(btn_minus, SIGNAL(clicked()), this, SLOT(decrease()));


    setGeometry(x, y, w, h);
}

void Counter::increase()
{
    if (val == upper)
        return;
    val++;
    lbl->setText(to_string(val).c_str());
}

void Counter::decrease()
{
    if (val == lower)
        return;
    val--;
    lbl->setText(to_string(val).c_str());
}

int Counter::get_value()
{
    return val;
}

// ***************************************************
// Complexity widget
// ***************************************************

Star::Star(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed, int star_id)
: ToggleButton(parent1, x, y, w, h, icon_path, icon_path_pressed)
{
    id = star_id;
}

void Star::mousePressEvent(QMouseEvent *e)
{
    ((ComplexityWidget *) parentWidget())->set_complexity(id);
}

ComplexityWidget::ComplexityWidget(QWidget *parent, int x, int y, int h, int num_stars)
: QWidget(parent)
{
    max_compl = num_stars;
    cur_compl = max_compl / 2;

    for (int i = 0; i < num_stars; ++i)
    {
        stars[i] = new Star(this, h * i, 0, h, h,
                             ":img/star.svg",
                             ":img/star_on.svg", i);
        if (i < max_compl / 2)
            stars[i]->turn_on();
    }

    setGeometry(x, y, h * num_stars, h);
}

int ComplexityWidget::get_complexity()
{
    return cur_compl;
}

void ComplexityWidget::set_complexity(int complexity)
{
    cur_compl = complexity;
    int i;
    for (i = 0; i <= cur_compl; ++i)
        stars[i]->turn_on();
    for (; i < max_compl; ++i)
        stars[i]->turn_off();
}


// ***************************************************
// Rounded Label
// ***************************************************

RoundedLabel::RoundedLabel(QWidget *parent, int w, int h)
: QWidget(parent)
{
    lbl = new QLabel(this);
    lbl->setGeometry(0, 0, w, h);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    setFixedSize(w, h);
    setVisible(false);
}

void RoundedLabel::set_label(int x, int y, QString text, int font_size)
{
    move(x, y);
    lbl->setText(text);
    QFont font("Source Sans Pro", font_size);
    lbl->setFont(font);
    setVisible(true);
}

void RoundedLabel::paintEvent(QPaintEvent *e)
{
    //QLabel::paintEvent(e);
    QPainter qp(this);
    qp.setPen(QColor(240, 240, 240));
    qp.setBrush(QColor(240, 240, 240));
    qp.drawRoundedRect(0, 0, width(), height(), 3, 3);
}

string RoundedLabel::get_text()
{
    return lbl->text().toStdString();
}

// ***************************************************
// Tool Widget
// ***************************************************

ToolBtn::ToolBtn(QWidget *parent1, int x, int y, int w, int h, string icon_path, string icon_path_pressed,
                 int star_id)
: Star(parent1, x, y, w, h, icon_path, icon_path_pressed, star_id)
{
    ;
}

void ToolBtn::mousePressEvent(QMouseEvent *e)
{
    if (!state_on)
        svg_rnd->load(path_on);

    state_on = true;

    update();
    ((ToolWidget *) parentWidget()) ->set_tool(id);
}

ToolWidget::ToolWidget(QWidget *parent, int x, int y, int w, int h)
: QWidget(parent)
{
    cur_tool_id = 0;

    int mrg_x = w * 0.05;
    int space = (w - mrg_x * 2) / 20.0;
    int btn_side = (w - mrg_x * 2 - 5 * space) / (6);
    int mrg_y = (h - btn_side) / 2;

    tools[0] = new ToolBtn(this, mrg_x, mrg_y, btn_side, btn_side, ":img/tool1.svg",
                             ":img/tool1_on.svg", 0);
    tools[0]->turn_on();

    tools[1] = new ToolBtn(this, mrg_x + (btn_side + space) * 1, mrg_y, btn_side, btn_side, ":img/tool2.svg",
                             ":img/tool2_on.svg", 1);

    tools[2] = new ToolBtn(this, mrg_x +(btn_side + space) * 2, mrg_y, btn_side, btn_side, ":img/tool3.svg",
                             ":img/tool3_on.svg", 2);

    tools[3] = new ToolBtn(this, mrg_x +(btn_side + space) * 3, mrg_y, btn_side, btn_side, ":img/tool4.svg",
                             ":img/tool4_on.svg", 3);

    tools[4] = new ToolBtn(this, mrg_x +(btn_side + space) * 4, mrg_y, btn_side, btn_side, ":img/tool5.svg",
                             ":img/tool5_on.svg", 4);

    tools[5] = new ToolBtn(this, mrg_x +(btn_side + space) * 5, mrg_y, btn_side, btn_side, ":img/tool6.svg",
                             ":img/tool6_on.svg", 5);

    setGeometry(x, y, w, h);
}

void ToolWidget::set_tool(int id)
{
    cur_tool_id = id;

    for (int i = 0; i < 6; i++)
        if (i != id)
            tools[i]->turn_off();

    ((MainWindow *) parentWidget())->set_tool(cur_tool_id);
}

void ToolWidget::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    paint_background(qp, 0, 0, width(), height());
}


// ***************************************************
// CheckBox
// ***************************************************

CheckBox::CheckBox(QWidget *parent, int cb_id, int x, int y, int w, int h, string name, bool checked, bool cb_alone)
: QPushButton(parent)
{
    id = cb_id;
    alone = cb_alone;

    lbl = new QLabel(name.c_str(), this);
    lbl->move(w * 0.15, 0);
    lbl->setFixedHeight(h);
    lbl->setAlignment(Qt::AlignVCenter);
    lbl->setStyleSheet("color: #58595B");
    lbl->setFont(font11);

    int btn_h = h;
    btn_svg = new SVGButton(this, 0, 0, btn_h, btn_h, ":img/cb_off.svg");
    connect(btn_svg, SIGNAL(clicked()), this, SLOT(click()));
    if (checked)
        btn_svg->load_img(":img/cb_on.svg");

    is_on = checked;

    setGeometry(x, y, w, h);
}

bool CheckBox::is_checked()
{
    return is_on;
}

void CheckBox::turn_on()
{
    is_on = true;
    btn_svg->load_img(":img/cb_on.svg");
    update();
}

void CheckBox::turn_off()
{
    is_on = false;
    btn_svg->load_img(":img/cb_off.svg");
    update();
}

void CheckBox::click()
{
    is_on = ! is_on;
    if (is_on)
    {
        btn_svg->load_img(":img/cb_on.svg");
    } else
    {
        btn_svg->load_img(":img/cb_off.svg");
    }
    if (!alone)
        ((CheckBoxesWidget *) parentWidget())->update_widget(id);
    update();
}

void CheckBox::paintEvent(QPaintEvent *e)
{
    ;
}

// ***************************************************
// CheckBoxesWidget
// ***************************************************

CheckBoxesWidget::CheckBoxesWidget(QWidget *parent, int x, int y, int w, int h, string title, vector<string> names)
: QWidget(parent)
{
    cb_names = names;

    // Checkboxes
    int space = h * 0.2;
    int mrg_y = (h - space * names.size() - h * 0.3) / 2;
    for (int i = 0; i < names.size(); ++i)
    {
        checkboxes[i] = new CheckBox(this, i, w * 0.1, mrg_y + h * 0.3 + i * space, w, space, names[i].c_str());
    }
    checkboxes[0]->click();

    // Title lable

    lbl_title = new QLabel(title.c_str(), this);
    lbl_title->move(0, h * 0.1);
    lbl_title->setFixedSize(w, h * 0.2);
    lbl_title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    QFont font = font11;
    font.setBold(true);
    lbl_title->setStyleSheet(color_title_bold.c_str());
    lbl_title->setFont(font);

    setGeometry(x, y, w, h);
}

void CheckBoxesWidget::update_widget(int id_checked)
{
    cur_ind = id_checked;
    for (int i = 0; i < cb_names.size(); ++i)
    {
        if (i != id_checked)
            checkboxes[i]->turn_off();
    }
}

int CheckBoxesWidget::get_checked_index()
{
    return cur_ind;
}

void CheckBoxesWidget::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);

    paint_background(qp, 0, 0, width(), height());
}
