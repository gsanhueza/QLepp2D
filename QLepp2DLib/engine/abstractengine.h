#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include "off/offmetadata.h"
#include "structs/triangle.h"
#include "structs/vertex.h"

class AbstractEngine
{
public:
    AbstractEngine() {};
    virtual ~AbstractEngine() {};

    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices) = 0;
    virtual bool improveTriangulation(  std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata) = 0;

protected:
    virtual void setup() {};
};

#endif // OPENCLHANDLER_H
