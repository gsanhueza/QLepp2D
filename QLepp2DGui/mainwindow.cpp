#include <QDebug>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tutorial(new Tutorial),
    m_about(new About)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadTriangulationClicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("OFF files"), ".", tr("OFF Files (*.off)"));
    if (m_model.loadOFF(filepath))
    {
        // Saving current filename so we can use it as a hint for saving the OFF file
        QFileInfo fileinfo(filepath);
        m_currentFileName = fileinfo.completeBaseName();

        qDebug() << m_currentFileName << "triangulation loaded." << endl;
        ui->statusBar->showMessage("Loaded.");
        emit emitModel(m_model);
    }
    else
    {
        qWarning() << "Could not open file" << filepath;
        ui->statusBar->showMessage("Unable to load.");
    }

}

void MainWindow::saveTriangulationClicked()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("OFF files"), m_currentFileName, tr("OFF Files (*.off)"));

    if (not m_model.saveOFF(filepath))
    {
        qWarning() << "Could not save file" << filepath;
        ui->statusBar->showMessage("Unable to save.");
    }
    else
    {
        qDebug() << "Triangulation Saved.";
        ui->statusBar->showMessage("Saved.");
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

