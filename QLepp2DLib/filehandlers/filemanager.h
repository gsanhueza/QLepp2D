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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QMap>
#include <QString>

#include <string>
#include <vector>

#include <filehandlers/filehandler.h>

#include <structs/triangle.h>
#include <structs/vertex.h>
#include <structs/edge.h>
#include <structs/edgedata.h>

/**
* @brief Factory class for the file managing module.
*
*/
class FileManager
{
public:
    /**
     * @brief Constructor of FileManager.
     *
     */
    FileManager();

    /**
     * @brief Destructor of FileManager.
     *
     */
    ~FileManager();

    /**
     * @brief Adds a file handler to allow the library to load several types
     * of files.
     * @param handler p_handler: The class that is expected to handle the file loading.
     * @param extension p_extension: Extension of the file that this handler can... handle.
     * @return True if correctly added.
     */
    bool addFileHandler(FileHandler *handler, std::string extension);

    /**
    * @brief Method that calls a FileHandler to load a mesh file.
    *
    * @param filepath p_filepath: Path of the mesh file.
    * @param vertices p_vertices: Vector of vertices.
    * @param edges p_edges: Vector of edges.
    * @param triangles p_triangles: Vector of triangles.
    * @return True if correctly loaded.
    */
    bool load(std::string filepath,
              std::vector<Vertex> &vertices,
              std::vector<Edge> &edges,
              std::vector<Triangle> &triangles);

    /**
    * @brief Method that calls a FileHandler to save a mesh file.
    *
    * @param filepath p_filepath: Path of the mesh file.
    * @param vertices p_vertices: Vector of vertices.
    * @param edges p_edges: Vector of edges.
    * @param indices p_triangles: Vector of triangles.
    * @return True if correctly saved.
    */
    bool save(std::string filepath,
              std::vector<Vertex> &vertices,
              std::vector<Edge> &edges,
              std::vector<Triangle> &triangles);

private:
    QMap<QString, FileHandler*> m_handlers;

};

#endif // FILEMANAGER_H
