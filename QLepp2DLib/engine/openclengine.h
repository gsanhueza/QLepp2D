#ifndef OPENCLENGINE_H
#define OPENCLENGINE_H

#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
# include <OpenCL/cl.cpp>
#else
# include <CL/cl.hpp>
#endif

#include "engine/abstractengine.h"

class OpenCLEngine : public AbstractEngine
{
public:
    OpenCLEngine();

    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices);
    virtual bool improveTriangulation(  std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata);

protected:
    virtual void setup();

private:
    std::vector<cl::Platform> m_platforms;
    std::vector<cl::Device> m_devices;
    cl::Context m_context;
    cl::CommandQueue m_queue;
    cl::Program m_program;
};

#endif // OPENCLENGINE_H
