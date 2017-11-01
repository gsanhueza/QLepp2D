#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include "triangle.h"

class AbstractEngine
{
public:
    AbstractEngine();

    virtual bool detectBadTriangles(double &angle, std::vector<Triangle> &triangles) = 0;
    virtual bool improveTriangulation(std::vector<Triangle> &triangles) = 0;
};

#endif // OPENCLHANDLER_H
