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

#ifndef CPUENGINE_H
#define CPUENGINE_H

#include <engine/engine.h>

/**
 * @brief CPU Implementation of the Engine.
 *
 */
class CPUEngine : public Engine
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
     * @param vertices p_vertices: Vector of vertices.
     * @param triangles p_triangles: Vector of triangles.
     * @return True if detected without issues.
     */
    virtual bool detectBadTriangles(float angle,
                                    std::vector<Vertex> &vertices,
                                    std::vector<Triangle> &triangles) override;

    /**
     * @brief Improves the actual triangulation from the vector of triangles. Overridden method.
     *
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     * @return True if improved without issues.
     */
    virtual bool improveTriangulation(std::vector<Vertex> &vertices,
                                      std::vector<Edge> &edges,
                                      std::vector<Triangle> &triangles) override;

    /**
     * @brief Detects terminal edges for each bad triangle in the "triangles"
     * vector. Overridden method.
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
                                     bool &flag) override;

    /**
     * @brief Inserts centroids on every region that has a terminal edge.
     *
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     */
    virtual void insertCentroids(std::vector<Vertex> &vertices,
                                 std::vector<Edge> &edges,
                                 std::vector<Triangle> &triangles) override;

private:
    /**
     * @brief Returns the index to the "edges" vector in which the shared
     * (or border) terminal edge was found in the "it" triangle.
     *
     * @param it p_it: Index of the starting "bad" triangle.
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     * @param flag p_flag: Flag that shows if we still have Non-border Terminal Edges.
     * @return int Index of the terminal edge. -1 on error (Not expected to return an error).
     */
    int getTerminalIEdge(int it,
                         std::vector<Vertex> &vertices,
                         std::vector<Edge> &edges,
                         std::vector<Triangle> &triangles,
                         bool &flag) const;

    /**
     * @brief Returns the centroid of the 4 vertices.
     *
     * @param iva p_iva: Index of vertex a.
     * @param ivb p_ivb: Index of vertex b.
     * @param ivc p_ivc: Index of vertex c.
     * @param ivd p_ivd: Index of vertex d.
     * @param vertices p_vertices: Vector of vertices.
     * @return The centroid Vertex.
     */
    Vertex centroidOf(int iva,
                      int ivb,
                      int ivc,
                      int ivd,
                      std::vector<Vertex> &vertices);

    /**
     * @brief Inserts the centroid of the 2 triangles marked by index "iedge".
     *
     * @param iedge p_iedge: Index of terminal edge.
     * @param vertices p_vertices: Vector of vertices.
     * @param edges p_edges: Vector of edges.
     * @param triangles p_triangles: Vector of triangles.
     */
    void insertCentroid(int iedge,
                        std::vector<Vertex> &vertices,
                        std::vector<Edge> &edges,
                        std::vector<Triangle> &triangles);
};

#endif // CPUENGINE_H
