#include <QApplication>
#include "cpuengine.h"
#include "openclengine.h"
#include "mainwindow.h"
#include "model.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    AbstractEngine *engine(new OpenCLEngine);
    Model model(engine);

    MainWindow window;
    window.setModel(&model);

    window.show();

    return app.exec();
}
