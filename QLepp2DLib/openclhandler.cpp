#include "openclhandler.h"

OpenCLHandler::OpenCLHandler()
{

}

bool OpenCLHandler::detectBadTriangles(int &angle, std::vector<Triangle> &triangles)
{
    // TODO
    triangles.at(0).bad = 1;
    return true;
}
