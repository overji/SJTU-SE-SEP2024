#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Create the main window and show it
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
