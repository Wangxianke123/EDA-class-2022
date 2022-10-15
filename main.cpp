#include "mainwindow/mainwindow.h"
#include <QApplication>

int main(int argc, char **args)
{
    QApplication app(argc, args);

    MainWindow *mainwindow = new MainWindow;

    mainwindow->show();

    return app.exec();
}

