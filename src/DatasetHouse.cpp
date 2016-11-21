//
// Created by Igor Kuznetsov on 07/08/16.
//

#include <QPainter>
#include "DatasetHouse.h"
#include "MainWindow.h"
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <fstream>

// ***************************************************
// Icon
// ***************************************************

Icon::Icon(QWidget *parent)
: QWidget(parent)
{
    id = -1;
    lbl_name = new QLabel(this);
    lbl_name->setFont(font11);
    lbl_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    svg_icon = new SVGButton(this, 0, 0, 0, 0, ":img/icon2.svg");
    setVisible(false);
}

void Icon::set_icon(int x, int y, int w, int h, int id_icon, string name, int num_img_types)
{
    id = id_icon;

    QString q_name = name.c_str();
    lbl_name->setText(q_name.toUpper());
    lbl_name->setFixedSize(w * 0.7, h);
    lbl_name->move(w * 0.05, 0);

    string icon_path = ":img/icon" + to_string(num_img_types) + ".svg";
    svg_icon->setGeometry(w * 0.8, h * 0.1, h * 0.8, h * 0.8);
    svg_icon->load_img(icon_path.c_str());

    setGeometry(x, y, w, h);
    setVisible(true);
}

void Icon::mousePressEvent(QMouseEvent *e)
{
    ((DatasetHouse *) parentWidget())->show_ds_info(id);
}

void Icon::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.setPen(QColor(220, 220, 220));
    qp.setBrush(QColor(220, 220, 220));
    qp.drawRoundedRect(0, 0, width(), height(), 2, 2);
}


// ***************************************************
// DatasetHouse
// ***************************************************

DatasetHouse::DatasetHouse(QWidget *parent, int x, int y, int w, int h)
: QWidget(parent)
{
    wnd_x = x;
    wnd_y = y;
    wnd_w = w;
    wnd_h = h;

    mrg_x = w * 0.05;
    mrg_y = h * 0.13;

    setGeometry(0, 0, ((MainWindow *) parentWidget())->width(),
                ((MainWindow *) parentWidget())->height());

    // Title label
    icon_side = w * 0.02;
    lbl_title = new QLabel(this);
    lbl_title->setText("DATASET HOUSE");
    lbl_title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    lbl_title->move(x + (w - lbl_title->width()) / 2 + icon_side, y + h * 0.04);

    lbl_title->setStyleSheet(color_title_bold.c_str());
    QFont font("Source Sans Pro", 14);
    font.setBold(true);
    lbl_title->setFont(font);

    // SVG icon
    svg_icon = new SVGButton(this, x + (w - icon_side - lbl_title->width()) / 2, y + h * 0.04,
                             icon_side, icon_side,
                             ":img/box.svg");

    // DS name
    int space_h = wnd_h * 0.04;
    lbl_ds_name = new QLabel("NAME:", this);
    lbl_ds_name->move(wnd_x + wnd_w * 0.77, wnd_y + wnd_h * 0.13);
    lbl_ds_name->setFixedWidth(w * 0.2);
    lbl_ds_name->setFont(QFont("Source Sans Pro", 15));
    lbl_ds_name->setStyleSheet("color: #666666");

    // DS size
    lbl_ds_size = new QLabel("100 IMG", this);
    lbl_ds_size->move(wnd_x + wnd_w * 0.77, wnd_y + wnd_h * 0.13 + space_h * 1);
    lbl_ds_size->setFixedWidth(w * 0.2);
    lbl_ds_size->setFont(QFont(font11));
    lbl_ds_size->setStyleSheet("color: #666666");

    // Complexity stars
    int star_side = w * 0.015;
    for (int i = 0; i < 7; i++)
    {
        stars[i] = new SVGButton(this, wnd_x + wnd_w * 0.77 + star_side * i, wnd_y + wnd_h * 0.13 + space_h * 2, star_side, star_side,
                                 ":img/star.svg");
    }

    // DS grid size
    lbl_grid_size = new QLabel("GRID SIZE", this);
    lbl_grid_size->move(wnd_x + wnd_w * 0.77, wnd_y + wnd_h * 0.1 + space_h * 4);
    lbl_grid_size->setFixedWidth(w * 0.2);
    lbl_grid_size->setFont(font12);
    lbl_grid_size->setStyleSheet("color: #666666");

    // DS OUTPUTS
    lbl_outputs = new QLabel("IMAGE LABELS", this);
    lbl_outputs->move(wnd_x + wnd_w * 0.77, wnd_y + wnd_h * 0.1 + space_h * 5);
    lbl_outputs->setFixedSize(w * 0.2, h * 0.2);
    lbl_outputs->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lbl_outputs->setFont(font12);
    lbl_outputs->setStyleSheet("color: #666666");

    // DS description
    lbl_desc = new QLabel("DESCRIPTION", this);
    lbl_desc->move(wnd_x + wnd_w * 0.77, wnd_y + wnd_h * 0.1 + space_h * 7.5);
    lbl_desc->setFixedWidth(w * 0.2);
    lbl_desc->setFont(QFont(font12));
    lbl_desc->setStyleSheet("color: #666666");

    lbl_desc_text = new QLabel("DESCRIPTION", this);
    lbl_desc_text->move(wnd_x + wnd_w * 0.77, wnd_y + wnd_h * 0.1 + space_h * 9);
    lbl_desc_text->setFixedSize(w * 0.2, h * 0.3);
    lbl_desc_text->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    lbl_desc_text->setFont(font12);
    lbl_desc_text->setStyleSheet("color: #666666");
    lbl_desc_text->setWordWrap(true);

    // Close button
    btn_close = new PressedButton(this, x + w * 0.973, y + w * 0.005, w * 0.023, w * 0.023,
                                  ":img/delete.svg", ":img/delete_on.svg");
    connect(btn_close, SIGNAL(clicked()), this, SLOT(hide_window()));

    // Confirm button
    int btn_w = w * 0.1;
    btn_confirm = new TextButton(this, x + w * 0.87 - btn_w / 2, y + h * 0.9, btn_w, h * 0.04, "CONFIRM");
    connect(btn_confirm, SIGNAL(clicked()), this, SLOT(confirm_dataset()));

    // Button "Set working directory"
    btn_set_wrk_dir = new TextButton(this, x + w * 0.48, y + h * 0.9, w * 0.12, h * 0.04, "Set working dir");
    connect(btn_set_wrk_dir, SIGNAL(clicked()), this, SLOT(set_wrk_dir()));

    // Button "Open file"
    btn_open_file = new TextButton(this, x + w * 0.605, y + h * 0.9, w * 0.1, h * 0.04, "Open external");
    connect(btn_open_file, SIGNAL(clicked()), this, SLOT(open_file()));

    // Scan working directory for listing all datasets

    char str[255];
    ds_id_count = 0;

    // Load integrated datasets

    string path = ":/data";
    QDir cur_dir(path.c_str());
    QStringList file_names = cur_dir.entryList();

    // ! VARIABLE OF THE NUMBER OF INTERNAL DATASETS
    num_internal_datasets = 6;

            foreach (QFileInfo file, file_names)
        {
            if (!file.isDir()) {
                process_ds_file(ds_id_count++, path + "/" + file.fileName().toStdString());
            }
        }

    // Create icons
    icon_width = w * 0.13;
    icon_height = icon_width * 0.22;
    for (int i = 0; i < 50; ++i)
    {
        icons[i] = new Icon(this);
    }

    for (int i = 0; i < ds_info.size(); ++i)
    {
        icons[i]->set_icon(x + mrg_x + (i % 5) * icon_width * 1.05, y + mrg_y + (icon_height * 1.2) * (i / 5),
                icon_width, icon_height, i, ds_info[i].name, ds_info[i].num_img_types);
    }

    set_visible_preview_widets(false);
}

int DatasetHouse::process_ds_file(int id, string path)
{
    QFile fInput(path.c_str());
    fInput.open(QIODevice::ReadOnly);
    QTextStream sin(&fInput);

    QString line;

    try {

        // Reading name
        line = sin.readLine();
        while (line.compare("[name]") && !line.isEmpty())
            line = sin.readLine();

        if (line.isEmpty())
        {
            cout << "Wrong file " << path << endl;
            return 0;
        }

        line = sin.readLine();
        string name = line.toStdString();

        // Reading grid size
        while (line.compare("[grid_size]"))
            line = sin.readLine();
        line = sin.readLine();
        int grid_size = line.toInt();

        // Reading complexity
        while (line.compare("[complexity]"))
            line = sin.readLine();
        line = sin.readLine();
        int complexity = line.toInt();

        // Reading outputs
        vector<string> outputs;
        while (line.compare("[outputs_start]"))
            line = sin.readLine();
        line = sin.readLine();
        do {
            outputs.push_back(line.toStdString());
            line = sin.readLine();
        } while (line.compare("[outputs_end]"));

        // Reading description
        string desc = "";
        while (line.compare("[description]"))
            line = sin.readLine();
        line = sin.readLine();
        do {
            desc += line.toStdString() + string("\n");
            line = sin.readLine();
        } while (line.compare("[output]"));

        // close file stream
        fInput.close();

        // Add new description
        if (id >= num_internal_datasets)
            ds_info.push_back(ds_preview_info(id, name, wrk_dir_path + "/" + name, outputs.size(), grid_size, complexity, outputs, desc));
        else
            ds_info.push_back(ds_preview_info(id, name, ":/data/" + name, outputs.size(), grid_size, complexity, outputs, desc));
        return 1;
    }
    catch (...)
    {
        cout << "Error" << endl;
        return 0;
    }
}

void DatasetHouse::confirm_dataset()
{
    ((MainWindow *) parentWidget())->load_dataset(ds_info[id_selected_ds].path);
    hide_window();
}

void DatasetHouse::hide_window()
{
    set_visible_preview_widets(false);
    setVisible(false);
}

void DatasetHouse::show_ds_info(int ds_id)
{
    // set id of the selected ds-file
    id_selected_ds = ds_id;

    // Make widgets visible for the case of the the fist opening
    set_visible_preview_widets(true);

    // set widgets to the according ds-info
    QString q_buf = ("NAME:  " + ds_info[id_selected_ds].name).c_str();
    lbl_ds_name->setText(q_buf.toUpper());
    q_buf = ("GRID SIZE:  " + to_string(ds_info[id_selected_ds].grid_size)).c_str();
    lbl_grid_size->setText(q_buf);
    q_buf = "IMAGE LABELS:  ";
    for (int i = 0; i < ds_info[id_selected_ds].num_img_types; ++i)
    {
        q_buf.append(ds_info[id_selected_ds].outputs[i].c_str());
        q_buf.append(" ");
        if (!((i+1) % 5))
            q_buf.append("\n");
    }
    lbl_outputs->setText(q_buf);
    q_buf = ds_info[id_selected_ds].desc.c_str();
    lbl_desc_text->setText(q_buf);

    // burn stars of comlexity;
    int i;
    for (i = 0; i < ds_info[id_selected_ds].complexity; ++i)
        stars[i]->load_img(":img/star_on.svg");
    for (; i < 7; ++i)
        stars[i]->load_img(":img/star.svg");
    update();
}

void DatasetHouse::set_visible_preview_widets(bool visbl)
{
    lbl_ds_name->setVisible(visbl);
    lbl_ds_size->setVisible(visbl);
    lbl_grid_size->setVisible(visbl);
    lbl_outputs->setVisible(visbl);
    lbl_desc->setVisible(visbl);
    lbl_desc_text->setVisible(visbl);
    btn_confirm->setVisible(visbl);
    for (int i = 0; i < 7; ++i)
        stars[i]->setVisible(visbl);
}

void DatasetHouse::open_file()
{
    QString file_name;
    file_name = QFileDialog::getOpenFileName(this, tr("Open Dataset File"));

    if (file_name.isEmpty())
        return;

    // Load file from parent widget (MainWindow)
    ((MainWindow *) parentWidget())->load_dataset(file_name.toStdString());
    hide_window();
}

void DatasetHouse::set_wrk_dir()
{
    QString dir_name;
    dir_name = QFileDialog::getExistingDirectory(this, tr("Save to Directory"),
                                                 QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir_name.isEmpty())
        return;

    wrk_dir_path = dir_name.toStdString();

    QDir wrk_dir(dir_name);
    QStringList file_names = wrk_dir.entryList();
    bool success_load;

            foreach (QFileInfo file, file_names)
        {
            if (!file.isDir()) {
                success_load = process_ds_file(ds_id_count, dir_name.toStdString() + "/" + file.fileName().toStdString());

                // Add new icon
                if (success_load) {
                    icons[ds_id_count]->set_icon(wnd_x + mrg_x + (ds_id_count % 5) * icon_width * 1.05,
                                                  wnd_y + mrg_y + (icon_height * 1.2) * (ds_id_count / 5),
                                                  icon_width, icon_height, ds_id_count, ds_info[ds_id_count].name,
                                                  ds_info[ds_id_count].num_img_types);
                    ds_id_count++;
                }

                // Limit for the number of datasets in the system is 50
                if (ds_id_count == 50)
                    return;
            }
        }

    // User have permission set working directory only once per
    // application running
    btn_set_wrk_dir->setVisible(false);

    update();
}

void DatasetHouse::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);

    // Background
    qp.setBrush(QColor(210, 210, 210, 200));
    qp.drawRect(0, 0, width(), height());

    // Window
    qp.setPen(Qt::white);
    qp.setBrush(Qt::white);
    qp.drawRoundedRect(wnd_x, wnd_y, wnd_w, wnd_h, 3, 3);

    // Dotted line
    int dot_step = (wnd_h - wnd_h * 0.2) / 70;
    QPen pen = QPen(QColor(210, 210, 210));
    pen.setWidth(2);
    qp.setPen(pen);
    for (int i = 0; i < 75; ++i)
    {
        qp.drawPoint(wnd_x + wnd_w * 0.75, wnd_y + wnd_h * 0.14 + dot_step * i);
    }
}