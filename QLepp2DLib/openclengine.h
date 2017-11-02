#ifndef OPENCLENGINE_H
#define OPENCLENGINE_H

# define __CL_ENABLE_EXCEPTIONS
# if defined(__APPLE__) || defined(__MACOSX)
#   include <OpenCL/cl.cpp>
# else
#   include <CL/cl.hpp>
# endif

# include "abstractengine.h"

class OpenCLEngine : public AbstractEngine
{
public:
    OpenCLEngine();

    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices,
                                    std::vector<int> &indices);
    virtual bool improveTriangulation(  std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata);

protected:
    virtual void setup();

private:
    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;
    cl::CommandQueue queue;
    cl::Program program;
};

#endif // OPENCLENGINE_H
