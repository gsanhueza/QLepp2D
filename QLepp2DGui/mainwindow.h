#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tutorial.h"
#include "about.h"
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setModel(Model *model);

signals:
    void emitModel(Model *m);
    void resetView();

private slots:
    void loadTriangulationClicked();
    void saveTriangulationClicked();
    void reloadViewClicked();
    void loadTutorialClicked();
    void loadAboutClicked();
    void detectClicked();
    void improveClicked();

private:
    Ui::MainWindow *ui;
    Tutorial *m_tutorial;
    About *m_about;

    Model *m_model;
    QString m_currentFileName;
};

#endif // MAINWINDOW_H
