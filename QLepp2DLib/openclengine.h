#ifndef OPENCLENGINE_H
#define OPENCLENGINE_H

#include "abstractengine.h"

class OpenCLEngine : public AbstractEngine
{
public:
    OpenCLEngine();

    virtual bool detectBadTriangles(double &angle, std::vector<Triangle> &triangles);
    virtual bool improveTriangulation(std::vector<Triangle> &triangles);
};

#endif // OPENCLENGINE_H
