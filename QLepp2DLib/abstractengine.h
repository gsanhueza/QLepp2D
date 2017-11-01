#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include "offmetadata.h"
#include "triangle.h"

class AbstractEngine
{
public:
    AbstractEngine();

    virtual bool detectBadTriangles(double &angle, std::vector<Triangle> &triangles) = 0;
    virtual bool improveTriangulation(std::vector<Triangle> &triangles, OFFMetadata &metadata) = 0;
};

#endif // OPENCLHANDLER_H
