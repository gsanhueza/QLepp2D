#ifndef OFFHANDLER_H
#define OFFHANDLER_H

#include <QString>
#include <vector>
#include "off/offmetadata.h"
#include "structs/triangle.h"
#include "structs/vertex.h"
#include "qlepp2dlib_global.h"

class QLEPP2DLIBSHARED_EXPORT OFFHandler
{

public:
    OFFHandler();
    bool loadOffFile(QString &filepath, OFFMetadata &metadata, std::vector<Vertex> &vertices, std::vector<int> &indices, std::vector<Triangle> &triangles);
    bool saveOffFile(QString &filepath, OFFMetadata &metadata, std::vector<Vertex> &vertices, std::vector<int> &indices) const;

private:
    int m_numVertices;
    int m_numFaces;
    int m_numEdges;
};

#endif // OFFHANDLER_H
