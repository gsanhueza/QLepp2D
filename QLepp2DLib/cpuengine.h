#ifndef CPUENGINE_H
#define CPUENGINE_H

#include "abstractengine.h"

class CPUEngine : public AbstractEngine
{
public:
    CPUEngine();

    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices,
                                    std::vector<int> &indices);
    virtual bool improveTriangulation(  std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata);
};

#endif // CPUENGINE_H
