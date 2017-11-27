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

#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include "off/offmetadata.h"
#include "structs/triangle.h"
#include "structs/vertex.h"
#include "structs/edge.h"

/**
* @brief Abstract class for engines. Each implementation must override at least
* the methods detectBadTriangles and improveTriangulation.
*
*/
class AbstractEngine
{
public:
    /**
    * @brief Non-abstract Constructor.
    *
    */
    AbstractEngine() {};

    /**
    * @brief Virtual destructor.
    *
    */
    virtual ~AbstractEngine() {};

    /**
    * @brief Detects every bad triangle in the vector of triangles. Implementations must override this.
    *
    * @param angle p_angle: Tolerance angle.
    * @param triangles p_triangles: Vector of triangles.
    * @param vertices p_vertices: Vector of vertices.
    * @return True if detected without issues.
    */
    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices) = 0;


    /**
    * @brief Improves the actual triangulation from the vector of triangles. Implementations must override this.
    *
    * @param triangles p_triangles: Vector of triangles.
    * @param vertices p_vertices: Vector of vertices.
    * @param indices p_indices: Vector of indices.
    * @param metadata p_metadata: Metadata for the (potentially saved) OFF file.
    * @return True if improved without issues.
    */
    virtual bool improveTriangulation(  std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        std::vector<Edge> &edges,
                                        OFFMetadata &metadata) = 0;

protected:
    /**
    * @brief Convenience method that helps when an implementation needs something done before working.
    *
    */
    virtual void setup() {};
};

#endif // OPENCLHANDLER_H
