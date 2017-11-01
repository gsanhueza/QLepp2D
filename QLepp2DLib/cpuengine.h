#ifndef CPUENGINE_H
#define CPUENGINE_H

#include "abstractengine.h"

class CPUEngine : public AbstractEngine
{
public:
    CPUEngine();

    virtual bool detectBadTriangles(double &angle, std::vector<Triangle> &triangles);
    virtual bool improveTriangulation(std::vector<Triangle> &triangles);
};

#endif // CPUENGINE_H
