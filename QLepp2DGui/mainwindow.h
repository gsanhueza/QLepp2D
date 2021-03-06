/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017-2019 Gabriel Sanhueza <gabriel_8032@hotmail.com>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <tutorial.h>
#include <about.h>
#include <model.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model *model, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void emitUpdateModel(Model *model);
    void emitUpdateData();
    void resetView();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void loadTriangulationClicked();
    void saveTriangulationClicked();
    void resetViewClicked();
    void loadTutorialClicked();
    void loadAboutClicked();
    void loadAboutQtClicked();
    void detectClicked();
    void improveClicked();
    void cpuEngineClicked();
    void openclEngineClicked();

private:
    void loadFile(QString path);
    void saveFile(QString path);
    void readSettings();
    void writeSettings();
    void addRecentFile(QString path);
    void removeRecentFile(QString path);
    void updateRecentFiles();
    void clearRecentFiles();

private:
    Ui::MainWindow *ui;
    Tutorial *m_tutorial;
    About *m_about;
    QSettings *m_settings;
    QStringList m_recentFilesList;

    QString m_currentFileName;
    const int m_recentFilesLimit;

    Model *m_model;
};

#endif // MAINWINDOW_H
