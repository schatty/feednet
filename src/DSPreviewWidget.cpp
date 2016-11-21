//
// Created by Igor Kuznetsov on 02/08/16.
//

#include <QPainter>

#include "DSPreviewWidget.h"
#include "gui_properties.h"

PageLabel::PageLabel(QWidget *parent, int x, int y, int w, int h, char *text, int btn_id)
: TextButton(parent, x, y, w, h, text)
{
    id = btn_id;
}

void PageLabel::mousePressEvent(QMouseEvent *e)
{
    TextButton::mousePressEvent(e);

    ((DSPreviewWidget *) parentWidget())->set_page(id);
}

// ***************************************************
// DSPreviewWidget
// ***************************************************

DSPreviewWidget::DSPreviewWidget(QWidget *parent, int x, int y, int w, int h)
: QWidget(parent)
{
    data_initialized = false;

    int h_btn = h * 0.04;

    for (int i = 0; i < 11; i++)
    {
        btn_lbl_page[i] = new PageLabel(this, (w * 0.1 + w * 0.01) * i, 0, w * 0.1, h_btn, "ALL", i);
        pages[i] = new PagePreview(this, 0, h * 0.05, w, h - h_btn * 2);
        if (i)
        {
            btn_lbl_page[i]->setVisible(false);
            pages[i]->setVisible(false);
        }

        // Create all-tab labels
        lbls_all_tab[i] = new QLabel(this);
        lbls_all_tab[i]->setFont(font11);
        lbls_all_tab[i]->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        lbls_all_tab[i]->setVisible(false);
    }
    pages[0]->set_img_name("ALL");

    setGeometry(x, y, w, h);
}

void DSPreviewWidget::init_widget(vector<string> outputs, multimap<string, vector<unsigned char>> *d1, int grid_size)
{
    data = d1;
    data_initialized = true;
    num_pages = outputs.size() + 1;
    img_names = outputs;

    int i;
    for (i = 1; i < num_pages; ++i)
    {
        btn_lbl_page[i]->setVisible(true);
        QString lbl_text = outputs[i - 1].c_str();
        btn_lbl_page[i]->set_text(lbl_text);

        pages[i]->set_img_name(outputs[i-1]);
        pages[i]->set_img_size(grid_size);
    }
    for (; i < 10; i++)
    {
        btn_lbl_page[i]->setVisible(false);
    }

    // Set img size to the ALL tab images
    pages[0]->set_img_size(grid_size);

    // Set all-tab labels
    int img_column_side = width() / (num_pages - 1);
    for (i = 0; i < num_pages - 1; ++i)
    {
        lbls_all_tab[i]->move(i * img_column_side, height() * 0.06);
        lbls_all_tab[i]->setFixedWidth(img_column_side);
        QString lbl_text = outputs[i].c_str();
        lbls_all_tab[i]->setText(lbl_text);
    }
    for (; i < 10; i++)
    {
        lbls_all_tab[i]->setVisible(false);
    }
}

void DSPreviewWidget::set_page(int page_id)
{
    if (! data_is_ready()) {
        return;
    }

    for (int i = 0; i < num_pages; i++)
    {
        if (page_id == i)
        {
            pages[i]->setVisible(true);
        } else
        {
            pages[i]->setVisible(false);
        }
    }

    bool is_all_tab = !page_id;
    for (int i = 0; i < num_pages - 1; ++i)
        lbls_all_tab[i]->setVisible(is_all_tab);

}

bool DSPreviewWidget::data_is_ready()
{
    return data_initialized;
}

// ***************************************************
// PagePreview
// ***************************************************

PagePreview::PagePreview(QWidget *parent, int x, int y, int w, int h)
: QWidget(parent)
{
    page_on = false;
    img_name = "";

    // Graphic vars
    mrg_x = w * 0.02;
    mrg_y = w * 0.02;
    num_img_per_line = 15;
    cur_line = 0;
    // !
    space_between = 32;

    setGeometry(x, y, w, h);
}

void PagePreview::set_img_name(string img_nm)
{
    img_name = img_nm;
}

void PagePreview::set_img_size(int size)
{
    img_size = size;
}

void PagePreview::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);

    // background
    paint_background(qp, 0, 0, width(), height());

    // this operations may be comp. expensive

    if (! ((DSPreviewWidget *) parentWidget())->data_is_ready())
        return;

    pair<multimap<string, vector<unsigned char>>::iterator, multimap<string, vector<unsigned char>>::iterator> ii;
    multimap<string, vector<unsigned char>>::iterator i;


    if (! img_name.compare("ALL"))
    {
        int num_img = ((DSPreviewWidget *) parentWidget())->num_pages - 1;

        int num_img_per_line_in_column = num_img_per_line / num_img;
        int img_column_side = (width() - mrg_x * 2) / num_img;

        for (int j = 0; j < ((DSPreviewWidget *) parentWidget())->img_names.size(); ++j)
        {
            string img = ((DSPreviewWidget *) parentWidget())->img_names[j];
            ii = ((DSPreviewWidget *) parentWidget())->data->equal_range(img);

            int x = mrg_x + j * img_column_side;
            int y = mrg_y * 3;
            int im_num;

            for (i = ii.first, im_num = 1; i != ii.second; ++i, ++im_num)
            {
                draw_image(&qp, i->second, x, y);

                if (! (im_num % num_img_per_line_in_column))
                {
                    x = mrg_x + j * img_column_side;
                    y += space_between;
                } else
                    x += space_between;
            }
        }

    } else {

        ii = ((DSPreviewWidget *) parentWidget())->data->equal_range(img_name);

        int x = mrg_x;
        int y = mrg_y;
        int im_num;

        for (i = ii.first, im_num = 1; i != ii.second; ++i, ++im_num)
        {
            draw_image(&qp, i->second, x, y);

            if (! (im_num % num_img_per_line))
            {
                x = mrg_x;
                y += space_between;
            } else
                x += space_between;
        }
    }

}

void PagePreview::draw_image(QPainter *qp, vector<unsigned char> v, int x, int y)
{
    for (int i = 0; i < img_size; i++)
        for (int j = 0; j < img_size; j++)
        {
            unsigned char c = v[i * img_size + j];
            qp->setPen(QColor(c, c, c));
            qp->drawPoint(x + j, y + i);
        }
}
