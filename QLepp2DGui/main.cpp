#include <QApplication>
#include "mainwindow.h"
#include "model.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Model model;

    MainWindow window;
    window.setModel(&model);

    window.show();

    return app.exec();
}
