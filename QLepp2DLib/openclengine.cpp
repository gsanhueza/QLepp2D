#include <QDebug>
#include <QFile>
#include "openclengine.h"

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
        std::vector<double> angleVector;
        angleVector.push_back(angle);
        // Create the memory buffers (Implicit copy to buffers when using iterators)
        // true == Read Only
        cl::Buffer bufferAngle(m_context, angleVector.begin(), angleVector.end(), true);
        cl::Buffer bufferTriangles(m_context, triangles.begin(), triangles.end(), false);
        cl::Buffer bufferVertices(m_context, vertices.begin(), vertices.end(), true);

        // Make kernel
        cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> detect_kernel(m_program, "detectBadTriangles");

        // Set dimensions
        cl::NDRange global(triangles.size());
        //     cl::NDRange local( 256 );
        cl::EnqueueArgs eargs(m_queue, global/*, local*/);

        // Execute the kernel
        detect_kernel(eargs, bufferAngle, bufferTriangles, bufferVertices);

        // Copy the output data back to the host
        cl::copy(m_queue, bufferTriangles, triangles.begin(), triangles.end());
    }
    catch (cl::Error err)
    {
        qDebug() << err.err();
        return false;
    }
    return true;
}

bool OpenCLEngine::improveTriangulation(std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata)
{
    // TODO OpenCL improveTriangulation
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
    m_queue = cl::CommandQueue(m_context, m_devices[device_id]);

    // Read the program source from QRC, to avoid loading it from the relative path of the GUI executable
    QFile kernelfile(":/kernels/kernel.cl");
    kernelfile.open(QIODevice::ReadOnly);
    std::string kernel_code = kernelfile.readAll().toStdString();
    kernelfile.close();

    // create the program that we want to execute on the device
    cl::Program::Sources source;
    source.push_back({kernel_code.c_str(), kernel_code.length()});

    // Make program from the source code
    m_program = cl::Program(m_context, source);

    // Build the program for the devices
    m_program.build(m_devices);
}
