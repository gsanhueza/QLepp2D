#include <QDebug>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tutorial(new Tutorial),
    m_about(new About),
    m_model(new Model)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_tutorial;
    delete m_about;
    delete m_model;
    delete ui;
}

void MainWindow::loadTriangulationClicked()
{
    qDebug() << "Load Triangulation button clicked";
    QUrl filepath = QFileDialog::getOpenFileUrl(this, tr("OFF files"), QUrl(""), tr("OFF Files (*.off)"));
    m_model->loadOFF(filepath);
}

void MainWindow::saveTriangulationClicked()
{
    qDebug() << "Load Triangulation button clicked";
    QUrl filepath = QFileDialog::getSaveFileUrl(this, tr("OFF files"), QUrl(""), tr("OFF Files (*.off)"));
    m_model->saveOFF(filepath);
}

void MainWindow::loadTutorialClicked()
{
    qDebug() << "Load Tutorial button clicked";
    m_tutorial->show();
}

void MainWindow::loadAboutClicked()
{
    qDebug() << "Load About button clicked";
    m_about->show();
}
