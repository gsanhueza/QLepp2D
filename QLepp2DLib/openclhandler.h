#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include "triangle.h"

class OpenCLHandler
{
public:
    OpenCLHandler();

    bool detectBadTriangles(double &angle, std::vector<Triangle> &triangles);
    bool improveTriangulation(std::vector<Triangle> &triangles);
};

#endif // OPENCLHANDLER_H
