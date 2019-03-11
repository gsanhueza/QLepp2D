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

#ifndef OPENCLENGINE_H
#define OPENCLENGINE_H

#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
# include <OpenCL/opencl.hpp>
#else
# include <CL/cl.hpp>
#endif

#include <engine/engine.h>

/**
 * @brief OpenCL Implementation of the Engine.
 *
 */
class OpenCLEngine : public Engine
{
public:
    /**
     * @brief OpenCLEngine constructor.
     *
     */
    OpenCLEngine();

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

protected:
    /**
     * @brief Convenience method that sets variables up before work.
     *
     */
    void setup();

    /**
     * @brief Retrieves data from the current OpenCL Implementation.
     * @param platform_id: Platform ID
     * @param device_id: Device ID
     * @return Vector with data from the implementation (vendor, version, etc).
     */
    std::vector<std::string> getOpenCLData(unsigned long platform_id,
                                           unsigned long device_id) const;

private:
    std::vector<cl::Platform> m_platforms;
    std::vector<cl::Device> m_devices;
    cl::Context m_context;
    cl::CommandQueue m_queue;
    cl::Program m_program;

    cl::Buffer m_bufferVertices;
    cl::Buffer m_bufferEdges;
    cl::Buffer m_bufferTriangles;
};

#endif // OPENCLENGINE_H
