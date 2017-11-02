#ifndef OPENCLENGINE_H
#define OPENCLENGINE_H

#include "abstractengine.h"

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
};

#endif // OPENCLENGINE_H
