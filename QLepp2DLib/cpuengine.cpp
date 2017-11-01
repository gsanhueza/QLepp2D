#include <QDebug>
#include "cpuengine.h"

CPUEngine::CPUEngine()
{
}

bool CPUEngine::detectBadTriangles(double &angle, std::vector<Triangle> &triangles)
{
    // TODO
    qDebug() << "CPUEngine::detectBadTriangles - angle =" << angle;
    triangles.at(0).bad = 1;
    return true;
}

bool CPUEngine::improveTriangulation(std::vector<Triangle> &triangles, OFFMetadata &metadata)
{
    // TODO
    qDebug() << "CPUEngine::improveTriangulation";
    metadata.vertices = 1000;
    metadata.indices = 1000;
    metadata.edges = 1000;
    triangles.at(0).bad = 0;
    return true;
}
