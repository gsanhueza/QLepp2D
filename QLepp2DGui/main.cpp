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

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"
#include "model.h"
#include "engine/cpuengine.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Model model;

    QTranslator translator;
    translator.load(QString("qlepp2d_%1").arg(QLocale::system().name().split("_").at(0)), ":/i18n/");
    app.installTranslator(&translator);

    MainWindow window;
    window.setModel(&model);

    window.show();

    return app.exec();
}
