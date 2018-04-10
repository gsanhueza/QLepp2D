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

#include <QDebug>
#include <QFile>
#include "engine/openclengine.h"

OpenCLEngine::OpenCLEngine()
{
    setup();
}

bool OpenCLEngine::detectBadTriangles(  double &angle,
                                        std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices)
{
    qDebug() << "OpenCLEngine::detectBadTriangles - angle =" << angle;

    try
    {
        // Create the memory buffers (Implicit copy to buffers when using iterators)
        const bool USE_HOST_PTR = true;

        // true == CL_MEM_READ_ONLY / false == CL_MEM_READ_WRITE
        cl::Buffer bufferTriangles(m_context, triangles.begin(), triangles.end(), false, USE_HOST_PTR);
        cl::Buffer bufferVertices(m_context, vertices.begin(), vertices.end(), true, USE_HOST_PTR);

        // Make kernel
        cl::make_kernel<double&, cl::Buffer&, cl::Buffer&> detect_kernel(m_program, "detectBadTriangles");

        // Set dimensions
        cl::NDRange global(triangles.size());
        //cl::NDRange local( 256 );

        cl_ulong time_start = 0;
        cl_ulong time_end = 0;

        cl::EnqueueArgs eargs(m_queue, global/*, local*/);

        // Execute the kernel
        cl::Event event = detect_kernel(eargs, angle, bufferTriangles, bufferVertices);
        event.wait();

        event.getProfilingInfo(CL_PROFILING_COMMAND_START, &time_start);
        event.getProfilingInfo(CL_PROFILING_COMMAND_END, &time_end);

        qDebug() << "OpenCL: Bad Triangles detected in" << time_end - time_start << "nanoseconds.";
        // Copy the output data back to the host
        cl::copy(m_queue, bufferTriangles, triangles.begin(), triangles.end());
    }
    catch (cl::Error err)
    {
        qDebug() << err.err();
        qDebug() << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices[0]).c_str();
        return false;
    }
    return true;
}

void OpenCLEngine::detectTerminalEdgesBuffered(unsigned long globalSize,
                                               cl::Buffer &bufferTriangles,
                                               cl::Buffer &bufferVertices,
                                               cl::Buffer &bufferEdges)
{
    cl_ulong time_start = 0;
    cl_ulong time_end = 0;

    // Set dimensions
    cl::NDRange global(globalSize);
    //cl::NDRange local(256);
    cl::EnqueueArgs eargs(m_queue, global/*, local*/);

    // Make kernel
    cl::make_kernel<cl::Buffer&, cl::Buffer&, cl::Buffer&> detect_terminal_edges_kernel(m_program, "detectTerminalEdges");

    // Execute the kernel
    cl::Event event = detect_terminal_edges_kernel(eargs, bufferTriangles, bufferVertices, bufferEdges);

    event.wait();

    event.getProfilingInfo(CL_PROFILING_COMMAND_START, &time_start);
    event.getProfilingInfo(CL_PROFILING_COMMAND_END, &time_end);

    qDebug() << "OpenCL: Terminal edges detected in" << time_end - time_start << "nanoseconds.";
}

bool OpenCLEngine::improveTriangulation(std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        std::vector<Edge> &edges,
                                        OFFMetadata &metadata)
{
    /* We'll do this in 3 phases:
     * Phase 1: Detect the terminal edges for each bad triangle.
     * Phase 2: Insert new triangle(s) at each terminal edge.
     * Phase 3: Recalculate bad triangles.
     */
    try
    {
        // Create the memory buffers (Implicit copy to buffers when using iterators)
        const bool USE_HOST_PTR = true;

        // Create the triangleHistory vector
        std::vector<int> triangleHistory;
        for (int j(0); j < 3; j++)
        {
            triangleHistory.push_back(-1);
        }

        // true == CL_MEM_READ_ONLY / false == CL_MEM_READ_WRITE
        cl::Buffer bufferTriangles(m_context, triangles.begin(), triangles.end(), false, USE_HOST_PTR);
        cl::Buffer bufferVertices(m_context, vertices.begin(), vertices.end(), false, USE_HOST_PTR);
        cl::Buffer bufferIndices(m_context, indices.begin(), indices.end(), false, USE_HOST_PTR);
        cl::Buffer bufferEdges(m_context, edges.begin(), edges.end(), false, USE_HOST_PTR);
        cl::Buffer bufferTriangleHistory(m_context, triangleHistory.begin(), triangleHistory.end(), false, USE_HOST_PTR);

        /* As we're using GPU, we can use CRCW in this particular situation,
         * because when a terminal (index of) edge is found, we can update
         * the information of the edge, so it knows that it's a terminal edge by
         * updating isTerminalEdge with 0 or 1, and each thread will write the
         * same value. Thus, we can avoid duplicated terminal edges like we
         * could've had if we had used a vector of triangles in which each
         * triangle had its terminal iedge.
         *
         * This means that "detect_terminal_edges_kernel" will concurrently
         * update the "edges" vector so we can know which edges from the
         * bad triangles are terminals.
         */

        // Phase 1
        detectTerminalEdgesBuffered(triangles.size(), bufferTriangles, bufferVertices, bufferEdges);

        // Copy the output data back to the host
        cl::copy(m_queue, bufferTriangles, triangles.begin(), triangles.end());
        cl::copy(m_queue, bufferVertices, vertices.begin(), vertices.end());
        cl::copy(m_queue, bufferIndices, indices.begin(), indices.end());
        cl::copy(m_queue, bufferEdges, edges.begin(), edges.end());

//         cl::make_kernel<cl::Buffer&, cl::Buffer&, cl::Buffer&> detect_kernel(m_program, "improveTriangulation");
//         detect_kernel(eargs, bufferTriangles, bufferVertices, bufferIndices);

        // TODO Delete this
        // Checking terminal edges
//         for (int i(0); i < edges.size(); i++)
//         {
//             qDebug() << "[OPENCL] Edge" << i << ": isTerminalEdge = " << edges.at(i).isTerminalEdge;
//         }

        // TODO Delete this
        for (Triangle &t : triangles)
        {
            t.bad = 0;
        }
    }
    catch (cl::Error err)
    {
        qDebug() << err.err();
        qDebug() << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices[0]).c_str();
        return false;
    }
    return true;
}

void OpenCLEngine::setup()
{
    qDebug() << "Executing OpenCLEngine::setup";
    try
    {
        // Platform = Vendor (Intel, Nvidia, AMD, etc).
        int platform_id = 0;

        // Device = Card identifier. If you have only one graphic card from a vendor, leave this at 0.
        int device_id = 0;

        // Query for platforms
        cl::Platform::get(&m_platforms);

        // Get a list of devices on this platform
        // Select the platform.
        m_platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_CPU, &m_devices);

        // Create a context
        m_context = cl::Context(m_devices);

        // Create a command queue with profiling enabled.
        // Select the device.
        m_queue = cl::CommandQueue(m_context, m_devices[device_id], CL_QUEUE_PROFILING_ENABLE);

        // Read the program source from QRC, to avoid loading it from the relative path of the GUI executable
        QFile kernelfile(":/kernels/kernel.cl");
        kernelfile.open(QIODevice::ReadOnly);
        std::string kernel_code = kernelfile.readAll().toStdString();
        kernelfile.close();

        // Create the program that we want to execute on the device
        cl::Program::Sources source;
        source.push_back({kernel_code.c_str(), kernel_code.length()});

        // Make program from the source code
        m_program = cl::Program(m_context, source);

        // Build the program for the devices
        m_program.build(m_devices);
    }
    catch (cl::Error &e)
    {
        qDebug() << e.err();
        qDebug() << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices[0]).c_str();
        throw e;
    }
}
