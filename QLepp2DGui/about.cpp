#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();

    int x = (screenWidth - this->width()) / 2;
    int y = (screenHeight - this->height()) / 2;
    this->move(x, y);
}

About::~About()
{
    delete ui;
}
