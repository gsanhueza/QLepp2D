#include <QDebug>
#include "openclhandler.h"

OpenCLHandler::OpenCLHandler()
{

}

bool OpenCLHandler::detectBadTriangles(double &angle, std::vector<Triangle> &triangles)
{
    // TODO
    qDebug() << "OpenCLHandler: angle =" << angle;
    triangles.at(0).bad = 1;
    return true;
}

bool OpenCLHandler::improveTriangulation(std::vector<Triangle> &triangles)
{
    // TODO
    triangles.at(0).bad = 0;
    return true;
}
