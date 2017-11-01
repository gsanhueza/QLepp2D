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
    // Each insertion does:
    // +1 to metadata.vertices
    // +2 to metadata.indices
    // +3 to metadata.edges
    qDebug() << "CPUEngine::improveTriangulation";
    triangles.at(0).bad = 0;
    return true;
}
