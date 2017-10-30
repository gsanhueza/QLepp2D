#include "tutorial.h"
#include "ui_tutorial.h"

Tutorial::Tutorial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);

    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();

    int x = (screenWidth - this->width()) / 2;
    int y = (screenHeight - this->height()) / 2;
    this->move(x, y);
}

Tutorial::~Tutorial()
{
    delete ui;
}
