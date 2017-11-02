#include <QApplication>
#include "cpuengine.h"
#include "mainwindow.h"
#include "model.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow window;
    Model model(new CPUEngine);
    window.setModel(&model);

    window.show();

    return app.exec();
}
