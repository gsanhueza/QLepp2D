#include <fstream>
#include <QDebug>
#include "openclengine.h"

OpenCLEngine::OpenCLEngine()
{
    setup();
}

bool OpenCLEngine::detectBadTriangles(  double &angle,
                                        std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices)
{
    // FIXME
    qDebug() << "OpenCLEngine::detectBadTriangles - angle =" << angle;

    // Create the memory buffers (Implicit copy to buffers)
    cl::Buffer bufferTriangles(m_context, triangles.begin(), triangles.end(), true);
    cl::Buffer bufferVertices(m_context, vertices.begin(), vertices.end(), true);

    // Make kernel
    cl::make_kernel<cl::Buffer&, cl::Buffer&> detect_kernel(m_program, "detectBadTriangles");

    // Enqueue the kernel arguments
    cl::NDRange global(triangles.size());
    //     cl::NDRange local( 256 );
    cl::EnqueueArgs eargs(m_queue, global/*, local*/);

    // Execute the kernel
    detect_kernel(eargs, bufferTriangles, bufferVertices);

    // Copy the output data back to the host
    cl::copy(m_queue, bufferTriangles, triangles.begin(), triangles.end());

    return true;
}

bool OpenCLEngine::improveTriangulation(std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata)
{
    // TODO
    qDebug() << "OpenCLEngine::improveTriangulation";
    triangles.at(0).bad = 0;
    return true;
}

void OpenCLEngine::setup()
{
    qDebug() << "Executing OpenCLEngine::setup";

    int platform_id = 0;
    int device_id = 0;

    // Query for platforms
    cl::Platform::get(&m_platforms);

    // Get a list of devices on this platform
    // Select the platform.
    m_platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU|CL_DEVICE_TYPE_CPU, &m_devices);

    // Create a context
    m_context = cl::Context(m_devices);

    // Create a command queue
    // Select the device.
    m_queue = cl::CommandQueue( m_context, m_devices[device_id]);

    // Read the program source
    std::ifstream sourceFile("kernel.cl");
    std::string sourceCode( std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
    cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

    // Make program from the source code
    m_program = cl::Program(m_context, source);

    // Build the program for the devices
    m_program.build(m_devices);
}
