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

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

#include <structs/triangle.h>
#include <structs/vertex.h>
#include <structs/edge.h>

/**
* @brief Interface for files handling module.
* (Strategy Pattern)
*
*/
class FileHandler
{
public:
    /**
    * @brief Constructor of FileHandler.
    *
    */
    FileHandler() = default;

    /**
    * @brief Virtual destructor.
    *
    */
    virtual ~FileHandler() = default;

    /**
    * @brief Method that loads an OFF file and modifies the parameters according to the loaded triangulation.
    *
    * @param filepath p_filepath: Path of the OFF file.
    * @param vertices p_vertices: Vector of vertices.
    * @param edges p_edges: Vector of edges.
    * @param triangles p_triangles: Vector of triangles.
    * @return True if correctly loaded.
    */
    virtual bool load(std::string filepath,
                      std::vector<Vertex> &vertices,
                      std::vector<Edge> &edges,
                      std::vector<Triangle> &triangles) = 0;

    /**
    * @brief Method that saves an OFF file according to the actual parameters.
    *
    * @param filepath p_filepath: Path of the OFF file.
    * @param vertices p_vertices: Vector of vertices.
    * @param edges p_edges: Vector of edges.
    * @param indices p_triangles: Vector of triangles.
    * @return True if correctly saved.
    */
    virtual bool save(std::string filepath,
                      std::vector<Vertex> &vertices,
                      std::vector<Edge> &edges,
                      std::vector<Triangle> &triangles) = 0;
};

#endif // FILEHANDLER_H
