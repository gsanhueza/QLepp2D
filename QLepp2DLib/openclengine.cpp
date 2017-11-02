#include <QDebug>
#include "openclengine.h"

OpenCLEngine::OpenCLEngine()
{
    setup();
}

bool OpenCLEngine::detectBadTriangles(  double &angle,
                                        std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices)
{
    // TODO
    qDebug() << "OpenCLEngine::detectBadTriangles - angle =" << angle;
    triangles.at(0).bad = 1;
    return true;
}

bool OpenCLEngine::improveTriangulation(std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata)
{
    // TODO
    qDebug() << "OpenCLEngine::improveTriangulation";
    triangles.at(0).bad = 0;
    return true;
}

void OpenCLEngine::setup()
{
    qDebug() << "Executing OpenCLEngine::setup";
}
