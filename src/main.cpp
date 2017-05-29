#include "mainwindow.h"
#include <QApplication>

// for testing
#include "addarunconfiguration.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
