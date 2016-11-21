//
// Created by Igor Kuznetsov on 28/07/16.
//
#include <list>
#include <string>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>

#include "DrawingArea.h"
#include "visualization.h"
#include "CharactPanel.h"
#include "dataset_widget.h"
#include "DSPreviewWidget.h"
#include "windows.h"
#include "data_api.h"
#include "Network.h"
#include "DatasetHouse.h"
#include "VisualNet.h"
#include "gui_properties.h"

#ifndef NN_INTERFACE_MAINWINDOW_H
#define NN_INTERFACE_MAINWINDOW_H

using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT

    dataset *ds;

    int grid_size;
    unsigned char m[31][31];
    // Output labels
    list<string> output_lbls;
    // Training data
    list<int> train_data;
    // Type of the pen
    int tool;
    // Teaching parameters
    int current_era;
    // Data characteristics
    string data_name;
    list<int> img_type_sizes;

    // Current directory
    string cur_dir;

    // Network
    NetworkWithInterface *net;

    // Widgets
    DrawingArea *drawing_area;
    CharactPanel *charact_panel;
    dataset_widget *ds_widget;
    ToolWidget *tool_widget;
    DSPreviewWidget *ds_preview_widget;
    VisualNet *visual_net;
    Histogram *histogram;
    ProgressPlot *progress_plot;
    bool working_on_existing_ds;

    // Buttons to switch between DS and visualisation
    TextButton *btn_visual, *btn_data_preview;

    // Windows
    WindowNewDataset *wnd_new_dataset;
    DatasetHouse *wnd_datasets;
    WindowAbout *wnd_about;
    HelpAssistantLayer *layer_help;

    // stuff for status bar
    SVGButton *icon_status;
    QLabel *lbl_status;
    SVGButton *btn_help;
    SVGButton *btn_about;

    // Graphical stuff
    int bar_height;

public:
    MainWindow();
    unsigned char get_cell(int i, int j) { return m[i][j]; }
    void pick_cell(int i, int j);
    void set_pixel(int i, int j, unsigned char intensity);
    void set_tool(int tool_id);

    void create_dataset();
    void save_dataset();
    void load_dataset(string path);

    void add_image(string img_name);
    void change_image(int img_index);

    void recognize_image();
    void train_network();

    void set_message(char *message_text, int type);
    void set_message(string message_text, int type);

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void open_new_dataset_window();
    void open_dataset_house_window();
    void open_dataset_preview();
    void open_visual_net();
    void open_help_layer();
    void open_about_window();

    friend class DrawingArea;
    friend class dataset_widget;
    friend class HelpAssistantLayer;
};

#endif
