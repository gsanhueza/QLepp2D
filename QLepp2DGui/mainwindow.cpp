/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017  Gabriel Sanhueza <gabriel_8032@hotmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMimeData>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tutorial(new Tutorial(this)),
    m_about(new About(this)),
    m_settings(new QSettings("QLepp2D", "qlepp2d", this))
{
    ui->setupUi(this);

    setAcceptDrops(true);

    // Application centering
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();

    int x = (screenWidth - this->width()) / 2;
    int y = (screenHeight - this->height()) / 2;

    this->move(x, y);

    // Toolbar actions
    QAction *load = ui->mainToolBar->addAction(ui->actionLoadTriangulation->icon(), ui->actionLoadTriangulation->text());
    QAction *save = ui->mainToolBar->addAction(ui->actionSaveTriangulation->icon(), ui->actionSaveTriangulation->text());
    QAction *reset = ui->mainToolBar->addAction(ui->actionResetView->icon(), ui->actionResetView->text());
    QAction *quit = ui->mainToolBar->addAction(ui->actionQuit->icon(), ui->actionQuit->text());

    // Toolbar status tips
    load->setStatusTip(ui->actionLoadTriangulation->statusTip());
    save->setStatusTip(ui->actionSaveTriangulation->statusTip());
    reset->setStatusTip(ui->actionResetView->statusTip());
    quit->setStatusTip(ui->actionQuit->statusTip());

    // Toolbar connections
    connect(load, &QAction::triggered, this, &MainWindow::loadTriangulationClicked);
    connect(save, &QAction::triggered, this, &MainWindow::saveTriangulationClicked);
    connect(reset, &QAction::triggered, this, &MainWindow::resetViewClicked);
    connect(quit, &QAction::triggered, this, &MainWindow::close);

    // Default title
    setWindowTitle(windowTitle() + " (CPU)");
}

MainWindow::~MainWindow()
{
    // Save last opened files before closing
    m_settings->setValue("recentFiles", QStringList(m_recentFilesStack.toList()));

    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "MainWindow::dragEnterEvent";
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug() << "MainWindow::dropEvent";
    QString filepath = event->mimeData()->urls().at(0).toLocalFile();
    loadFile(filepath);
}

void MainWindow::loadFile(QString path)
{
    ui->statusBar->showMessage(tr("Loading..."));
    if (Model::getInstance().loadOFF(path))
    {
        // Saving current filename so we can use it as a hint for saving the OFF file
        QFileInfo fileinfo(path);
        m_currentFileName = fileinfo.completeBaseName();
        m_recentFilesStack.push(fileinfo.absoluteFilePath());
        m_settings->setValue("lastDir", fileinfo.absolutePath());

        qDebug() << m_currentFileName << "triangulation loaded." << endl;
        ui->statusBar->showMessage(tr("Loaded."));
        ui->improveButton->setDisabled(true);
        ui->detectButton->setEnabled(true);
        emit emitUpdateData();
    }
    else
    {
        qWarning() << "Could not open file" << path;
        ui->statusBar->showMessage(tr("Unable to load."));
    }
}

void MainWindow::loadTriangulationClicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("OFF files"), m_settings->value("lastdir", ".").toString() , tr("OFF Files (*.off)"));
    loadFile(filepath);
}

void MainWindow::saveTriangulationClicked()
{
    // Check that we can save something already loaded
    if (m_currentFileName == "")
    {
        qWarning() << "Nothing to save.";
        ui->statusBar->showMessage(tr("Nothing to save."));
        return;
    }

    QString filepath = QFileDialog::getSaveFileName(this, tr("OFF files"), m_currentFileName, tr("OFF Files (*.off)"));

    if (not Model::getInstance().saveOFF(filepath))
    {
        qWarning() << "Could not save file" << filepath;
        ui->statusBar->showMessage(tr("Unable to save."));
    }
    else
    {
        qDebug() << "Triangulation Saved.";
        ui->statusBar->showMessage(tr("Saved."));
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

void MainWindow::resetViewClicked()
{
    qDebug() << "Reset View button clicked";
    emit resetView();
}

void MainWindow::detectClicked()
{
    qDebug() << "Detect button clicked";
    double angle = ui->angleSpinBox->value();

    if (Model::getInstance().detectBadTriangles(angle))
    {
        ui->improveButton->setEnabled(true);
        ui->statusBar->showMessage(tr("Bad triangles have been detected. You can now proceed to improve them."));
        emit emitUpdateData();
    }
}

void MainWindow::improveClicked()
{
    qDebug() << "Improve button clicked";
    if (Model::getInstance().improveTriangulation())
    {
        ui->statusBar->showMessage(tr("Triangulation has been modified."));
        emit emitUpdateData();
    }
}

void MainWindow::cpuEngineClicked()
{
    qDebug() << "CPU Engine button clicked";
    if (Model::getInstance().setCPUEngine())
    {
        ui->statusBar->showMessage(tr("CPU Engine has been set."));
        setWindowTitle(tr("QLepp2D (CPU)"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Unable to set CPU engine."));
    }
}

void MainWindow::openclEngineClicked()
{
    qDebug() << "OpenCL Engine button clicked";
    if (Model::getInstance().setOpenCLEngine())
    {
        ui->statusBar->showMessage(tr("OpenCL Engine has been set."));
        setWindowTitle(tr("QLepp2D (OpenCL)"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Unable to set OpenCL engine."));
    }
}
