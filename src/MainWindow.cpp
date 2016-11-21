//
// Created by Igor Kuznetsov on 28/07/16.
//

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include <QDesktopServices>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>

#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow() : QWidget()
{
    // Create dataset storage
    ds = new dataset();

    // Set geometry of the window according to the screen size
    QRect rec = QApplication::desktop()->availableGeometry();
    int width = rec.width();
    int height = width / 1.768;

    bar_height = height * 0.04;

    setGeometry(0, 0, width, height);

    // Initialize gui properties
    init_gui_properties();

    // Panel labels and buttons
    icon_status = new SVGButton(this, width * 0.565, height * 0.007, width * 0.015, width * 0.015,
                                ":img/delim.svg");

    btn_help = new SVGButton(this, width * 0.03, height * 0.007, width * 0.015, width * 0.015, ":img/help.svg");
    connect(btn_help, SIGNAL(clicked()), this, SLOT(open_help_layer()));

    btn_about = new SVGButton(this, width * 0.01, height * 0.007, width * 0.015, width * 0.015, ":img/about.svg");
    connect(btn_about, SIGNAL(clicked()), this, SLOT(open_about_window()));

    // label for messages
    lbl_status = new QLabel(this);
    lbl_status->move(width * 0.585, 0);
    lbl_status->setFixedSize(width * 0.4, height * 0.04);
    lbl_status->setFont(QFont(font12));
    lbl_status->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lbl_status->setText("this is status board");

    // Size of the grid matrix
    grid_size = 28;

    // Fill in the grid matrix
    for (int i = 0; i < 31; i++)
        for (int j = 0; j < 31; j++)
            m[i][j] = 255;

    // Type of the pen
    tool = 0;

    // Teaching parameters
    current_era = 0;

    drawing_area = new DrawingArea(this, (int) width * 0.17, (int) height * 0.14, (int) width * 0.2, 28);

    charact_panel = new CharactPanel(this, width * 0.02, height * 0.09, (int) (width * 0.13), (int) (height * 0.4));

    ds_widget = new dataset_widget(this, width * 0.375, height * 0.1, width * 0.18, width * 0.23);

    tool_widget = new ToolWidget(this, width * 0.17, height * 0.09, width * 0.16, width * 0.024);

    histogram = new Histogram(this, width * 0.02, height * 0.54, width * 0.2, width * 0.23, "Recognition Results");

    progress_plot = new ProgressPlot(this, width * 0.24, height * 0.54, width * 0.35, width * 0.21,
                                     0, 30, 0, 1,
                                     "ACCURACY", 0, width * 0.2 * 0.4);

    ds_preview_widget = new DSPreviewWidget(this, width * 0.57, height * 0.12, width * 0.415, height * 0.87);
    ds_preview_widget->setVisible(false);

    visual_net = new VisualNet(this, width * 0.57, height * 0.12, width * 0.415, height * 0.8);

    // Buttons to switch between visualization and data preview
    btn_visual = new TextButton(this, width * 0.57, height * 0.08, width * 0.08, height * 0.035, "VISUAL NET");
    connect(btn_visual, SIGNAL(clicked()), this, SLOT(open_visual_net()));

    btn_data_preview = new TextButton(this, width * 0.57 + width * 0.08 * 1.05, height * 0.08, width * 0.08,
                                      height * 0.035, "DATA");
    connect(btn_data_preview, SIGNAL(clicked()), this, SLOT(open_dataset_preview()));

    // Windows
    int w = width * 0.25;
    int h = height * 0.65;
    wnd_new_dataset = new WindowNewDataset(this, width / 2 - w/2, height / 2 - h/2, w, h, "NEW DATASET");
    wnd_new_dataset->setVisible(false);

    int wnd_w = width * 0.7;
    int wnd_h = height * 0.8;
    wnd_datasets = new DatasetHouse(this, (width - wnd_w) / 2, (height - wnd_h) / 2, wnd_w, wnd_h);
    wnd_datasets->hide_window();

    wnd_about = new WindowAbout(this,  width / 2 - w/2, height / 2 - h/2, w, h * 0.7);
    wnd_about->hide_window();

    layer_help = new HelpAssistantLayer(this);

    // Neural Network
    vector<int> sizes = {784, 10, 5};
    net = new NetworkWithInterface(sizes);
    visual_net->init_net(sizes);

    // Load dataset
    load_dataset(":/data/Digits_1_to_5");
    working_on_existing_ds = true;

    set_message("Welcome to Feednet", 1);
}

void MainWindow::pick_cell(int i, int j)
{
    switch (tool)
    {
        case 0:
            set_pixel(i, j, 50);
            set_pixel(i-1, j, 200);
            set_pixel(i-1, j-1, 200);
            set_pixel(i-1, j+1, 200);
            set_pixel(i, j-1, 200);
            set_pixel(i, j+1, 200);
            set_pixel(i+1, j-1, 200);
            set_pixel(i+1, j, 200);
            set_pixel(i+1, j+1, 200);
            break;
        case 1:
            set_pixel(i, j, 50);
            set_pixel(i-1, j, 200);
            set_pixel(i, j-1, 200);
            set_pixel(i, j+1, 200);
            set_pixel(i+1, j, 200);
            break;
        case 2:
            set_pixel(i, j, 50);
            set_pixel(i-1, j, 200);
            set_pixel(i, j+1, 200);
            break;
        case 3:
            set_pixel(i, j, 50);
            set_pixel(i+1, j, 50);
            set_pixel(i+1, j+1, 50);
            set_pixel(i, j+1, 50);
            break;
        case 4:
            set_pixel(i, j, 50);
            break;
        case 5:
            set_pixel(i, j, 255);
    }
    drawing_area->update();
}

void MainWindow::set_pixel(int i, int j, unsigned char intensity)
{
    if (i < 0 || j < 0 || i >= grid_size || j >= grid_size)
        return;

    if (m[i][j] == 50 && intensity != 255)
        return;
    m[i][j] = intensity;
}

void MainWindow::open_new_dataset_window()
{
    wnd_new_dataset->open();
}

void MainWindow::set_tool(int tool_id)
{
   tool = tool_id;
}

void MainWindow::create_dataset()
{
    // Create new DS in dataset storage
    string name = wnd_new_dataset->get_name().toStdString();
    vector<string> outputs = wnd_new_dataset->get_outputs();
    grid_size = wnd_new_dataset->get_grid_size();
    int compexity = wnd_new_dataset->get_complexity();
    string desc = wnd_new_dataset->get_desc().toStdString();

    ds = new dataset(name, outputs, grid_size, compexity, desc);

    // Set teaching mode to drawing widget
    drawing_area->set_teach_mode(grid_size, &outputs);

    // Set teaching mode to dataset widget
    QString ds_name = wnd_new_dataset->get_name();
    ds_widget->reset_widget();
    ds_widget->set_teaching_mode(ds_name, outputs);

    // Initialize dataset preview widget
    ds_preview_widget->init_widget(outputs, ds->get_data(), grid_size);

    // Now working on new one
    working_on_existing_ds = false;

    set_message("New dataset was created", 1);
}

void MainWindow::add_image(string img_name)
{
    // Get image from matrix
    vector<unsigned char> grid;

    for (int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
            grid.push_back(m[i][j]);

    // Update dataset structure
    ds->add_image(drawing_area->get_cur_lbl(), grid);

    // Update dataset widget
    ds_widget->add_img(img_name);

    // Upaget datset preview widget
    ds_preview_widget->update();

    // Send message
    set_message("Image added", 0);
}

void MainWindow::change_image(int img_index)
{
    // Update preview widget
    ds_preview_widget->set_page(img_index + 1);
}

void MainWindow::save_dataset()
{
    // Set widgets in recognition mode
    ds_widget->set_recogn_mode();
    drawing_area->set_recogn_mode();

    // Save on disk

    QString dir_name;
    dir_name = QFileDialog::getExistingDirectory(this, tr("Save to Directory"),
                    QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ofstream out_ds(dir_name.toStdString() + "/" + ds->get_name());
    if (!out_ds)
    {
        cout << "Can not open file for saving." << endl;
        return;
    }

    out_ds << "[name]" << endl;
    out_ds << ds->get_name() << endl;

    out_ds << "[grid_size]" << endl;
    out_ds << ds->get_grid_size() << endl;

    out_ds << "[complexity]" << endl;
    out_ds << ds->get_complexity() << endl;

    out_ds << "[outputs_start]" << endl;
    for (int i = 0; i < ds->get_outputs().size(); ++i)
        out_ds << ds->get_outputs()[i] << endl;
    out_ds << "[outputs_end]" << endl;

    out_ds << "[description]" << endl;
    out_ds << ds->get_desc() << endl;

    pair<multimap<string, vector<unsigned char>>::iterator, multimap<string, vector<unsigned char>>::iterator> ii;
    multimap<string, vector<unsigned char>>::iterator i;

    int pix_code;
    for (int i_nm = 0; i_nm < ds->get_outputs().size(); ++i_nm)
    {
        out_ds << "[output]\n" << ds->get_outputs()[i_nm] << endl;
        ii = ds->get_data()->equal_range(ds->get_outputs()[i_nm]);
        for (i = ii.first; i != ii.second; ++i)
        {
            for (int i_c = 0; i_c < i->second.size(); i_c++) {
                pix_code = ((double) i->second[i_c] / 25.5) - 1;
                if (pix_code < 0)
                    pix_code = 0;
                out_ds << pix_code;
                //cout << "Saving " << ds->get_outputs()[i_nm] << " " << (int) i->second[i_c] << " " << ((double) i->second[i_c] / 28.3);
            }
                //out_ds << i->second[i_c];
            out_ds << endl;
        }
    }
    out_ds << "[end]";
    out_ds.close();

    // Load saved dataset.
    // Yes, i know im doing double job, I'll fix it later
    load_dataset(dir_name.toStdString() + "/" + ds->get_name());

    set_message("Dataset was saved successfully", 1);
}

void MainWindow::load_dataset(string path)
{
    QDir dir;
    QFile fInput(path.c_str());
    fInput.open(QIODevice::ReadOnly);
    QTextStream sin(&fInput);
    sin.setCodec("UTF-8");

    QString line;

    string nm;
    int grid_sz;
    int complexity;
    string desc = "";
    string output;
    vector<string> outputs;

    line = sin.readLine();
    while (line.compare("[name]"))
        line = sin.readLine();
    line = sin.readLine();

    nm = line.toStdString();

    //cout << "Name = "  << nm << endl;


    while (line.compare("[grid_size]"))
        line = sin.readLine();
    line = sin.readLine();

    grid_sz = line.toInt();

    //cout << "Grid: " << grid_sz << endl;

    while (line.compare("[complexity]"))
        line = sin.readLine();
    line = sin.readLine();

    complexity = line.toInt();

    //cout << "Complexity: " << complexity << endl;

    while (line.compare("[outputs_start]"))
        line = sin.readLine();

    line = sin.readLine();
        do {
            outputs.push_back(line.toStdString());
            line = sin.readLine();
        } while (line.compare("[outputs_end]"));


    while (line.compare("[description]"))
        line = sin.readLine();


    line = sin.readLine();
    do {
        desc += line.toStdString() + string("\n");
        line = sin.readLine();
    } while (line.compare("[output]"));

    //cout << "Descr: " << desc << endl;


    // Create empty dataset
    ds = new dataset(nm, outputs, grid_sz, complexity, desc);
    char ch;
    vector<unsigned char> v;

    wchar_t std_line[30 * 30];

    while (line.compare("[end]"))
    {
        line = sin.readLine();
        output = line.toStdString();
        cout << "Reading output: " << output << endl;

        line = sin.readLine();
        QChar c;
        int pix_code;

        while (line.compare("[output]") && line.compare("[end]"))
        {
            v.clear();
            for (int i = 0; i < grid_sz * grid_sz; ++i) {
                pix_code = line[i].toLatin1() - 48;
                if (pix_code > 7)
                    v.push_back(255);
                else
                    v.push_back((unsigned char) round(pix_code * 28.3));
            }
            line = sin.readLine();
            ds->add_image(output, v);
        }
    }

    fInput.close();

    // Reset network structure
    vector<int> sizes;
    sizes.push_back(grid_sz * grid_sz);
    sizes.push_back(10);
    sizes.push_back(outputs.size());
    net->reshape(sizes, 0, 0, 0);

    // Initialize dataset preview widget
    ds_preview_widget->init_widget(ds->get_outputs(), ds->get_data(), ds->get_grid_size());
    ds_widget->init_widget(ds->get_name(), ds->get_total_num_img(), ds->get_complexity(), ds->get_desc());
    histogram->init_histogram(ds->get_outputs());
    visual_net->init_net(sizes);

    // show dataset
    visual_net->setVisible(false);
    ds_preview_widget->setVisible(true);
    ds_preview_widget->set_page(0);

    // send message
    set_message("set loaded", 1);
}

void MainWindow::recognize_image()
{
    ArrayXd a(grid_size * grid_size);

    for (int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
            a(i * grid_size + j) = (double) 1.0 - (m[i][j] / 255.0);

    ArrayXd ouputs_percents = net->feedforward(a);

    // Update widgets
    histogram->set_histogram(ouputs_percents);

}

void MainWindow::train_network()
{
    int epochs = charact_panel->get_epochs();
    int mini_batch_size = charact_panel->get_mini_batch_size();
    int alpha = charact_panel->get_learn_rate();
    double test_rate = charact_panel->get_test_rate();
    bool watch_process = charact_panel->get_watch_process();

    if (epochs == -1 || alpha == -1 || mini_batch_size == -1)
        return;

    // update net structure
    vector<bool> par = visual_net->get_parameters();
    net->reshape(visual_net->get_sizes(), par[0], par[1], par[2]);

    net->SGD(*(ds->get_ds_in_pairs()), epochs, mini_batch_size, alpha,
             test_rate, watch_process);

    // update widgets
    progress_plot->init_plot(net->get_progress());

    // Send message
    set_message("Net has been trained successfully", 1);
}

void MainWindow::open_dataset_house_window()
{
    wnd_datasets->setVisible(true);
}

void MainWindow::open_dataset_preview()
{
    ds_preview_widget->setVisible(true);
    visual_net->setVisible(false);
}

void MainWindow::open_visual_net()
{
    ds_preview_widget->setVisible(false);
    visual_net->setVisible(true);
}

void MainWindow::set_message(char *message_text, int type)
{
    switch (type)
    {
        case 0:
            // Neutral gray
            lbl_status->setStyleSheet("color: #666666");
            break;
        case 1:
            // Success green
            lbl_status->setStyleSheet("color: #448245");
            break;
        case 2:
            // Failure red
            lbl_status->setStyleSheet("color: #994444");
            break;
        default:
            lbl_status->setStyleSheet("color: #ffffff");
    }
    lbl_status->setText(message_text);
}

// i'll correct it later
void MainWindow::set_message(string message_text, int type)
{
        switch (type)
        {
            case 0:
                // Neutral gray
                lbl_status->setStyleSheet("color: #666666");
                break;
            case 1:
                // Success green
                lbl_status->setStyleSheet("color: #448245");
                break;
            case 2:
                // Failure red
                lbl_status->setStyleSheet("color: #994444");
                break;
            default:
                lbl_status->setStyleSheet("color: #ffffff");
        }
        lbl_status->setText(message_text.c_str());
}

void MainWindow::open_help_layer()
{
    layer_help->show();
}

void MainWindow::open_about_window()
{
    wnd_about->setVisible(true);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    // Background
    qp.setPen(Qt::white);
    qp.setBrush(Qt::white);
    qp.drawRect(0, 0, width(), height());

    // Top panel
    qp.setPen(color_light_gray);
    qp.setBrush(color_light_gray);
    qp.drawRect(0, 0, width(), bar_height);
}