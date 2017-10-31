#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include "triangle.h"

class OpenCLHandler
{
public:
    OpenCLHandler();

    bool detectBadTriangles(int &angle, std::vector<Triangle> &triangles);
};

#endif // OPENCLHANDLER_H
