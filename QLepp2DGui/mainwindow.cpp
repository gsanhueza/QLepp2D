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
    QString filepath = QFileDialog::getOpenFileName(this, tr("OFF files"), ".", tr("OFF Files (*.off)"));
    if (m_model->loadOFF(filepath))
    {
        // Saving current filename so we can use it as a hint for saving the OFF file
        QFileInfo fileinfo(filepath);
        m_currentFileName = fileinfo.completeBaseName();

        qDebug() << "Saving filename as " << m_currentFileName << endl;
    }
    else
    {
        qWarning() << "Could not open file" << filepath;
    }
}

void MainWindow::saveTriangulationClicked()
{
    qDebug() << "Load Triangulation button clicked";
    QString filepath = QFileDialog::getSaveFileName(this, tr("OFF files"), m_currentFileName, tr("OFF Files (*.off)"));

    if (not m_model->saveOFF(filepath))
    {
        qWarning() << "Could not save file" << filepath;
    }
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
