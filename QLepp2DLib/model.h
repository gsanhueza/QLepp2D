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

#ifndef MODEL_H
#define MODEL_H

//#include <qlepp2dlib_global.h>
#include <string>
#include <vector>

#include <structs/vertex.h>
#include <structs/edge.h>
#include <structs/triangle.h>

class ModelImpl;

/**
* @brief API for QLepp2D.
*
*/
//class QLEPP2DLIBSHARED_EXPORT Model
class Model
{
public:
    /**
     * @brief Model Constructor.
     *
     */
    Model();

    /**
    * @brief Convenience method that sets the CPU Engine.
    *
    * @return True if correctly set.
    */
    bool setCPUEngine();

    /**
     * @brief Convenience method that sets the OpenCL Engine.
     *
     * @return True if correctly set.
     */
    bool setOpenCLEngine();

    /**
    * @brief Loads a mesh file so the inner implementation can receive the triangles.
    *
    * @param filepath p_filepath: Path of the file.
    * @return True if correctly loaded.
    */
    bool loadFile(std::string filepath);

    /**
    * @brief Saves a mesh file in the provided filepath.
    *
    * @param filepath p_filepath: Path of the file.
    * @return True if correctly saved.
    */
    bool saveFile(std::string filepath);

    /**
    * @brief Gets a vector of Vertex which are being used by the implementation.
    *
    * @return std::vector< Vertex >& Reference to the actual vector of vertices.
    */
    std::vector<Vertex>& getVertices();

    /**
    * @brief Gets a vector of Edges which are being used by the implementation.
    *
    * @return std::vector< Edge >& Reference to the actual vector of edges.
    */
    std::vector<Edge>& getEdges();

    /**
    * @brief Gets a vector of Triangles which are being used by the implementation.
    *
    * @return std::vector< Triangle >& Reference to the actual vector of triangles.
    */
    std::vector<Triangle>& getTriangles();

    /**
    * @brief Detects every triangle in the vector of triangles whose minimum angle is lesser than the provided angle.
    *
    * @param angle p_angle: Provided angle.
    * @return True if method has run without issues.
    */
    bool detectBadTriangles(float angle);

    /**
    * @brief Improves the actual triangulation, modifying the vectors in the Model.
    *
    * @return True if method has run without issues.
    */
    bool improveTriangulation();

private:
    ModelImpl *m_impl;
};

#endif // MODEL_H
