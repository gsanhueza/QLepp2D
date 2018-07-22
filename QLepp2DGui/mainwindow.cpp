/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017-2018 Gabriel Sanhueza <gabriel_8032@hotmail.com>
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
#include <QMenu>
#include <QMimeData>
#include <QMessageBox>

#include <mainwindow.h>
#include <ui_mainwindow.h>

MainWindow::MainWindow(Model *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tutorial(new Tutorial(this)),
    m_about(new About(this)),
    m_settings(new QSettings("QLepp2D", "qlepp2d", this)),
    m_recentFilesLimit(9),
    m_model(model)
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

    // Load recent files
    readSettings();

    // Update recent files in menu
    updateRecentFiles();

    // Update reference to Model in OpenGLWidget
    emit emitUpdateModel(m_model);
}

MainWindow::~MainWindow()
{
    writeSettings();
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
    std::string spath = path.toStdString();
    if (m_model->loadFile(spath))
    {
        // Saving current filename so we can use it as a hint for saving the OFF file
        QFileInfo fileinfo(path);
        m_currentFileName = fileinfo.completeBaseName();
        addRecentFile(path);

        qInfo() << m_currentFileName << "triangulation loaded." << endl;
        ui->statusBar->showMessage(tr("Loaded."));
        ui->improveButton->setDisabled(true);
        ui->detectButton->setEnabled(true);

        emit emitUpdateData();
    }
    else
    {
        qWarning() << "Could not open file" << path;
        removeRecentFile(path);
        ui->statusBar->showMessage(tr("Unable to load."));
    }
    updateRecentFiles();
}

void MainWindow::saveFile(QString filepath)
{
    std::string fspath = filepath.toStdString();
    if (not m_model->saveFile(fspath))
    {
        qWarning() << "Could not save file" << filepath;
        ui->statusBar->showMessage(tr("Unable to save."));
    }
    else
    {
        qInfo() << "Triangulation Saved.";
        ui->statusBar->showMessage(tr("Saved."));
    }
}

void MainWindow::loadTriangulationClicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    tr("OFF files"),
                                                    m_settings->value("lastDir", ".").toString(),
                                                    tr("OFF Files (*.off)"));
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

    QString outpath = QString("%1/%2_mod").arg(m_settings->value("lastDir", ".").toString()).arg(m_currentFileName);
    QString filepath = QFileDialog::getSaveFileName(this,
                                                    tr("OFF files"),
                                                    outpath,
                                                    tr("OFF Files (*.off)"));
    saveFile(filepath);
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

void MainWindow::loadAboutQtClicked()
{
    qDebug() << "Load AboutQt button clicked";
    QApplication::aboutQt();
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

    if (m_model->detectBadTriangles(static_cast<float>(angle)))
    {
        ui->improveButton->setEnabled(true);
        ui->statusBar->showMessage(tr("Bad triangles have been detected. You can now proceed to improve them."));
        emit emitUpdateData();
    }
    else
    {
        ui->statusBar->showMessage(tr("Unable to detect bad triangles."));
        QMessageBox::critical(this,
                              tr("QLepp2D"),
                              tr("Unable to detect bad triangles."));
    }
}

void MainWindow::improveClicked()
{
    qDebug() << "Improve button clicked";
    if (m_model->improveTriangulation())
    {
        ui->statusBar->showMessage(tr("Triangulation has been modified."));
        emit emitUpdateData();
    }
    else
    {
        ui->statusBar->showMessage(tr("Unable to improve triangulation."));
        QMessageBox::critical(this,
                              tr("QLepp2D"),
                              tr("Unable to improve triangulation."));
    }
}

void MainWindow::cpuEngineClicked()
{
    qDebug() << "CPU Engine button clicked";
    if (m_model->setCPUEngine())
    {
        ui->statusBar->showMessage(tr("CPU Engine has been set."));
        setWindowTitle(tr("QLepp2D (CPU)"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Unable to set CPU engine."));
    }
    ui->improveButton->setDisabled(true);
}

void MainWindow::openclEngineClicked()
{
    qDebug() << "OpenCL Engine button clicked";
    if (m_model->setOpenCLEngine())
    {
        ui->statusBar->showMessage(tr("OpenCL Engine has been set."));
        setWindowTitle(tr("QLepp2D (OpenCL)"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Unable to set OpenCL engine."));
    }
    ui->improveButton->setDisabled(true);
}

void MainWindow::readSettings()
{
    // Read recent files from last session
    m_recentFilesList = m_settings->value("recentFiles", "").toStringList();
}

void MainWindow::writeSettings()
{
    // Save last opened files before closing
    m_settings->setValue("recentFiles", m_recentFilesList);
}

void MainWindow::addRecentFile(QString path)
{
    QFileInfo info(path);

    m_settings->setValue("lastDir", info.absoluteDir().absolutePath());
    /* If we opened a file twice or more during the same session, we don't
     * want it duplicated.
     */
    removeRecentFile(path);
    m_recentFilesList.push_front(path);

    while (m_recentFilesList.size() > m_recentFilesLimit)
    {
        m_recentFilesList.removeLast();
    }
}

void MainWindow::removeRecentFile(QString path)
{
    if (!m_recentFilesList.isEmpty() and m_recentFilesList.contains(path))
    {
        m_recentFilesList.removeAll(path);
    }
}

void MainWindow::updateRecentFiles()
{
    ui->menuRecentTriangulations->clear();

    if (!m_recentFilesList.isEmpty())
    {
        ui->menuRecentTriangulations->setEnabled(true);

        for (int i(0); i < m_recentFilesList.size(); i++)
        {
            QString recentFile(m_recentFilesList.at(i));
            QAction *recentFileAction = ui->menuRecentTriangulations->addAction(QString("&%1 %2").arg(i + 1).arg(recentFile));
            connect(recentFileAction, &QAction::triggered, this, [recentFile, this] () {loadFile(recentFile);});
        }
        ui->menuRecentTriangulations->addSeparator();

        QIcon clearHistoryIcon;
        QString iconThemeName = QStringLiteral("edit-clear-history");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            clearHistoryIcon = QIcon::fromTheme(iconThemeName);
        } else {
            clearHistoryIcon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }

        QAction *clearHistoryAction = ui->menuRecentTriangulations->addAction(clearHistoryIcon, tr("&Clear history"));
        connect(clearHistoryAction, &QAction::triggered, this, &MainWindow::clearRecentFiles);
    }
    else
    {
        ui->menuRecentTriangulations->setDisabled(true);
    }
}

void MainWindow::clearRecentFiles()
{
    m_recentFilesList.clear();
    updateRecentFiles();
}
