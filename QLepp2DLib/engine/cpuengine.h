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

#ifndef CPUENGINE_H
#define CPUENGINE_H

#include "engine/abstractengine.h"

/**
* @brief CPU Implementation of the AbstractEngine.
*
*/
class CPUEngine : public AbstractEngine
{
public:
    /**
    * @brief CPUEngine constructor.
    *
    */
    CPUEngine();

    /**
    * @brief Detects every bad triangle in the vector of triangles. Overriden method.
    *
    * @param angle p_angle: Tolerance angle.
    * @param triangles p_triangles: Vector of triangles.
    * @param vertices p_vertices: Vector of vertices.
    * @return True if detected without issues.
    */
    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices) override;

    /**
    * @brief Improves the actual triangulation from the vector of triangles. Overridden method.
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
                                        OFFMetadata &metadata) override;

private:
    /**
    * @brief Returns the index to the "edges" vector in which the shared (or border) terminal edge was found.
    *
    * @param it p_it: Index of the starting "bad" triangle.
    * @param triangles p_triangles: Triangles vector.
    * @param vertices p_vertices: Vertices vector.
    * @param edges p_edges: Edges vector.
    * @return int Index of the terminal edge. -1 on error (Not expected to return this).
    */
    int getTerminalEdge(    int it,
                            std::vector<Triangle> &triangles,
                            std::vector<Vertex> &vertices,
                            std::vector<Edge> &edges) const;
};

#endif // CPUENGINE_H
