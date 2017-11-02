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
    // TODO
    qDebug() << "OpenCLEngine::detectBadTriangles - angle =" << angle;
    triangles.at(0).bad = 1;
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
    cl::Platform::get(&platforms);

    // Get a list of devices on this platform
    // Select the platform.
    platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU|CL_DEVICE_TYPE_CPU, &devices);

    // Create a context
    cl::Context context(devices);

    // Create a command queue
    // Select the device.
    queue = cl::CommandQueue( context, devices[device_id]);

    // Read the program source
    std::ifstream sourceFile("kernel.cl");
    std::string sourceCode( std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
    cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

    // Make program from the source code
    program = cl::Program(context, source);

    // Build the program for the devices
    program.build(devices);
}
