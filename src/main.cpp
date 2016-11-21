#include <iostream>
#include <QApplication>
#include <QDesktopWidget>

#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Comment the line below if app isn't for retina display
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    MainWindow window;

    window.setWindowTitle("Feednet");
    window.show();

    return a.exec();
}