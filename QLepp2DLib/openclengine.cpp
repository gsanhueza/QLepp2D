#include "openclengine.h"

OpenCLEngine::OpenCLEngine()
{
}

bool OpenCLEngine::detectBadTriangles(double &angle, std::vector<Triangle> &triangles)
{
    // TODO
    qDebug() << "OpenCLEngine::detectBadTriangles - angle =" << angle;
    triangles.at(0).bad = 1;
    return true;
}

bool OpenCLEngine::improveTriangulation(std::vector<Triangle> &triangles)
{
    // TODO
    qDebug() << "OpenCLEngine::improveTriangulation";
    triangles.at(0).bad = 0;
    return true;
}
