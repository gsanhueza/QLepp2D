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

#include <QDebug>
#include <QElapsedTimer>
#include <QFile>

#include <engine/openclengine.h>
#include <engine/cpuengine.h>

OpenCLEngine::OpenCLEngine()
{
    m_angle = 0;
    setup();
}

bool OpenCLEngine::detectBadTriangles(float angle,
                                      std::vector<Vertex> &vertices,
                                      std::vector<Triangle> &triangles)
{
    qDebug() << "OpenCLEngine::detectBadTriangles - angle =" << angle;

    m_angle = angle;
    try
    {
        QElapsedTimer timer;
        timer.start();

        // Create the memory buffers (Implicit copy to buffers when using iterators)
        const bool USE_HOST_PTR = true;

        // true == CL_MEM_READ_ONLY / false == CL_MEM_READ_WRITE
        m_bufferTriangles = cl::Buffer(m_context, triangles.begin(), triangles.end(), false, USE_HOST_PTR);
        m_bufferVertices = cl::Buffer(m_context, vertices.begin(), vertices.end(), false, USE_HOST_PTR);

        // Make kernel
        cl::make_kernel<float&, cl::Buffer&, cl::Buffer&> detect_kernel(m_program, "detectBadTriangles");

        // Set dimensions
        cl::NDRange global(triangles.size());
        //cl::NDRange local( 256 );

        cl_ulong time_start = 0;
        cl_ulong time_end = 0;

        cl::EnqueueArgs eargs(m_queue, global/*, local*/);

        // Execute the kernel
        cl::Event event = detect_kernel(eargs, angle, m_bufferTriangles, m_bufferVertices);
        event.wait();

        event.getProfilingInfo(CL_PROFILING_COMMAND_START, &time_start);
        event.getProfilingInfo(CL_PROFILING_COMMAND_END, &time_end);

        qDebug() << "OpenCL Algorithm (DBT): " << (time_end - time_start) << "nanoseconds";

        // Copy the output data back to the host
        cl::copy(m_queue, m_bufferTriangles, triangles.begin(), triangles.end());

        qint64 elapsed = timer.nsecsElapsed();
        qDebug() << "+ OpenCL: Bad Triangles detected in" << elapsed << "nanoseconds.";
    }
    catch (cl::Error &err)
    {
        qDebug() << err.err();
        qDebug() << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices.at(0)).c_str();
        return false;
    }
    return true;
}

bool OpenCLEngine::improveTriangulation(std::vector<Vertex> &vertices,
                                        std::vector<Edge> &edges,
                                        std::vector<Triangle> &triangles)
{
    /* We'll do this in 3 phases:
     * Phase 1: Detect the terminal edges for each bad triangle.
     * Phase 2: Insert new triangle(s) at each terminal edge.
     * Phase 3: Recalculate bad triangles.
     */
    try
    {
        // Phase 1
        bool nonBTERemaining = false; // Flag that shows if we still have Non-border Terminal Edges.
        detectTerminalEdges(vertices, edges, triangles, nonBTERemaining);

        if (not nonBTERemaining)
        {
            return true;
        }

        // Phase 2
        insertCentroids(vertices, edges, triangles);

        // Phase 3
        detectBadTriangles(m_angle, vertices, triangles);

        /* NOTE: If we get a correct GPU insertion algorithm, we can do this
         * only once instead of copying back the results for each function.
         */
    }
    catch (cl::Error &err)
    {
        qDebug() << err.err();
        qDebug() << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices.at(0)).c_str();
        return false;
    }
    catch (...)
    {
        qWarning() << "Unknown error in OpenCLEngine::improveTriangulation";
        return false;
    }
    return true;
}

void OpenCLEngine::detectTerminalEdges(std::vector<Vertex> &vertices,
                                       std::vector<Edge> &edges,
                                       std::vector<Triangle> &triangles,
                                       bool &flag)
{
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

    QElapsedTimer timer;
    timer.start();

    cl_ulong time_start = 0;
    cl_ulong time_end = 0;

    // Create the memory buffers (Implicit copy to buffers when using iterators)
    const bool USE_HOST_PTR = true;

    // Detect number of threads
    unsigned long globalSize(triangles.size());

    // true == CL_MEM_READ_ONLY / false == CL_MEM_READ_WRITE
    m_bufferTriangles = cl::Buffer(m_context, triangles.begin(), triangles.end(), false, USE_HOST_PTR);
    m_bufferVertices = cl::Buffer(m_context, vertices.begin(), vertices.end(), false, USE_HOST_PTR);
    m_bufferEdges = cl::Buffer(m_context, edges.begin(), edges.end(), false, USE_HOST_PTR);

    // Hack to allow flag to be modified by kernel
    std::vector<int> flagVector;
    flagVector.push_back(flag);
    cl::Buffer bufferFlag(m_context, flagVector.begin(), flagVector.end(), false, USE_HOST_PTR);

    // Set dimensions
    cl::NDRange global(globalSize);
    //cl::NDRange local(256);
    cl::EnqueueArgs eargs(m_queue, global/*, local*/);

    // Make kernel
    cl::make_kernel<cl::Buffer&, cl::Buffer&, cl::Buffer&, cl::Buffer&> detect_terminal_edges_kernel(m_program, "detectTerminalEdges");

    // Execute the kernel
    cl::Event event = detect_terminal_edges_kernel(eargs, m_bufferTriangles, m_bufferVertices, m_bufferEdges, bufferFlag);

    // Copy the modified edges and the flag back to CPU
    cl::copy(m_queue, m_bufferEdges, edges.begin(), edges.end());
    cl::copy(m_queue, bufferFlag, flagVector.begin(), flagVector.end());
    flag = (flagVector.at(0) != 0);

    event.wait();

    event.getProfilingInfo(CL_PROFILING_COMMAND_START, &time_start);
    event.getProfilingInfo(CL_PROFILING_COMMAND_END, &time_end);

    qDebug() << "OpenCL Algorithm (DTE): " << (time_end - time_start) << "nanoseconds";

    qint64 elapsed = timer.nsecsElapsed();
    qDebug() << "+ OpenCL: Terminal edges detected in" << elapsed << "nanoseconds.";
}

void OpenCLEngine::insertCentroids(std::vector<Vertex> &vertices,
                                   std::vector<Edge> &edges,
                                   std::vector<Triangle> &triangles)
{
    /* At the moment we'll use a mixed approach.
     * Timing will be done in CPUEngine.
     */
    CPUEngine cpuengine; // Temporarily we'll use this for centroid insertion
    cpuengine.insertCentroids(vertices, edges, triangles);

    // To avoid inconsistencies, we'll update edge information to buffers in GPU
    const bool USE_HOST_PTR = true;
    m_bufferEdges = cl::Buffer(m_context, edges.begin(), edges.end(), false, USE_HOST_PTR);
}

void OpenCLEngine::setup()
{
    qDebug() << "Executing OpenCLEngine::setup";
    try
    {
        // Platform = Vendor (Intel, Nvidia, AMD, etc).
        unsigned long platform_id(1);

        // Device = Card identifier. If you have only one graphic card from a vendor, leave this at 0.
        unsigned long device_id(0);

        // Query for platforms
        cl::Platform::get(&m_platforms);

        // Get a list of devices on this platform
        // Select the platform.
        m_platforms.at(platform_id).getDevices(CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_CPU, &m_devices);

        // Print information from the Platform and Device IDs
        for (std::string s : getOpenCLData(platform_id, device_id))
        {
            qDebug() << QString::fromStdString(s);
        }

        // Create a context
        m_context = cl::Context(m_devices);

        // Create a command queue with profiling enabled.
        // Select the device.
        m_queue = cl::CommandQueue(m_context, m_devices.at(device_id), CL_QUEUE_PROFILING_ENABLE);

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
        qDebug() << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices.at(0)).c_str();
        throw e;
    }
}

std::vector<std::string> OpenCLEngine::getOpenCLData(unsigned long platform_id,
                                                     unsigned long device_id) const
{
    // Investigate platform
    std::vector<std::string> vec;
    std::string ss;
    std::string s;

    m_platforms.at(platform_id).getInfo(CL_PLATFORM_NAME, &s);
    ss.append("  Platform: ");
    ss.append(s.c_str());
    vec.push_back(ss);
    ss.clear();

    m_platforms.at(platform_id).getInfo(CL_PLATFORM_VENDOR, &s);
    ss.append("  Vendor: ");
    ss.append(s.c_str());
    vec.push_back(ss);
    ss.clear();

    m_platforms.at(platform_id).getInfo(CL_PLATFORM_VERSION, &s);
    ss.append("  Version: ");
    ss.append(s.c_str());
    vec.push_back(ss);
    ss.clear();

    // Investigate device
    m_devices.at(device_id).getInfo(CL_DEVICE_NAME, &s);
    ss.append("    Device: ");
    ss.append(s.c_str());
    vec.push_back(ss);
    ss.clear();

    m_devices.at(device_id).getInfo(CL_DEVICE_OPENCL_C_VERSION, &s);
    ss.append("    Version: ");
    ss.append(s.c_str());
    vec.push_back(ss);
    ss.clear();

    int i;
    m_devices.at(device_id).getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &i);
    ss.append("    Max. Compute Units: ");
    ss.append(std::to_string(i));
    vec.push_back(ss);
    ss.clear();

    size_t size;
    m_devices.at(device_id).getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &size);
    ss.append("    Local Memory Size: ");
    ss.append(std::to_string(size/1024));
    ss.append(" KB");
    vec.push_back(ss);
    ss.clear();

    m_devices.at(device_id).getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &size);
    ss.append("    Global Memory Size: ");
    ss.append(std::to_string(size/(1024*1024)));
    ss.append(" MB");
    vec.push_back(ss);
    ss.clear();

    m_devices.at(device_id).getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &size);
    ss.append("    Max Alloc Size: ");
    ss.append(std::to_string(size/(1024*1024)));
    ss.append(" MB");
    vec.push_back(ss);
    ss.clear();

    m_devices.at(device_id).getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &size);
    ss.append("    Max Work-group Total Size: ");
    ss.append(std::to_string(size));
    vec.push_back(ss);
    ss.clear();

    std::vector<size_t> d;
    m_devices.at(device_id).getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &d);
    ss.append("    Max Work-group Dims: (");
    for (std::vector<size_t>::iterator st = d.begin(); st != d.end(); st++)
    {
        ss.append(std::to_string(*st));
        ss.append(" x ");
    }
    ss.pop_back();
    ss.pop_back();
    ss.pop_back();
    ss.append(")");
    vec.push_back(ss);
    ss.clear();

    return vec;
}
