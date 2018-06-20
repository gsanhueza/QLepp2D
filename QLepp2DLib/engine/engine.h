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

#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <structs/triangle.h>
#include <structs/vertex.h>
#include <structs/edge.h>

/**
 * @brief Interface for engines.
 * Each implementation must override detectBadTriangles and improveTriangulation.
 * Other methods must be overridden to allow fine-grained control over the
 * triangulation improvements.
 * (Strategy Pattern)
 *
 */
class Engine
{
public:
    /**
     * @brief Non-abstract Constructor.
     *
     */
    Engine() = default;

    /**
     * @brief Virtual destructor.
     *
     */
    virtual ~Engine() = default;

    // Minimal library

    /**
     * @brief Detects every bad triangle in the vector of triangles.
     * Implementations must override this.
     *
     * @param angle p_angle: Tolerance angle.
     * @param vertices p_vertices: Vector of vertices.
     * @param triangles p_triangles: Vector of triangles.
     * @return True if detected without issues.
     */
    virtual bool detectBadTriangles(double angle,
                                    std::vector<Vertex> &vertices,
                                    std::vector<Triangle> &triangles) = 0;

    /**
     * @brief Improves the actual triangulation from the vector of triangles.
     * Implementations must override this.
     *
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     * @return True if improved without issues.
     */
    virtual bool improveTriangulation(std::vector<Vertex> &vertices,
                                      std::vector<Edge> &edges,
                                      std::vector<Triangle> &triangles) = 0;

    // Available for API

    /**
     * @brief Detects terminal edges for each bad triangle in the "triangles"
     * vector.
     *
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     * @param flag p_flag: Flag that marks if a non-border terminal edge still
     * exists.
     */
    virtual void detectTerminalEdges(std::vector<Vertex> &vertices,
                                     std::vector<Edge> &edges,
                                     std::vector<Triangle> &triangles,
                                     bool &flag) = 0;

    /**
     * @brief Inserts centroids on every region that has a terminal edge.
     *
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     */
    virtual void insertCentroids(std::vector<Vertex> &vertices,
                                 std::vector<Edge> &edges,
                                 std::vector<Triangle> &triangles) = 0;

protected:
    double m_angle;
};

#endif // ENGINE_H
