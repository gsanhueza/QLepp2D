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

#include <QFileInfo>

#include <filehandlers/filemanager.h>
#include <filehandlers/offhandler.h>

FileManager::FileManager()
{
    addFileHandler(new OFFHandler, "off");
}

FileManager::~FileManager()
{
    for (auto it : m_handlers.values())
    {
        delete(it);
    }
}

bool FileManager::addFileHandler(FileHandler *handler, std::string extension)
{
    if (extension.size() > 0)
    {
        m_handlers.insert(QString::fromStdString(extension), handler);
        return true;
    }
    return false;
}

bool FileManager::load(std::string filepath, std::vector<Vertex> &vertices, std::vector<Edge> &edges, std::vector<Triangle> &triangles)
{
    QFileInfo fileinfo(QString::fromStdString(filepath));
    QString ext = fileinfo.suffix();

    FileHandler *handler = m_handlers.value(ext);
    return (handler != nullptr and handler->load(filepath, vertices, edges, triangles));
}

bool FileManager::save(std::string filepath, std::vector<Vertex> &vertices, std::vector<Edge> &edges, std::vector<Triangle> &triangles)
{
    QFileInfo fileinfo(QString::fromStdString(filepath));
    QString ext = fileinfo.suffix();

    FileHandler *handler = m_handlers.value(ext);
    return (handler != nullptr and handler->save(filepath, vertices, edges, triangles));
}
