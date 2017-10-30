#ifndef OFFHANDLER_H
#define OFFHANDLER_H

#include <QString>
#include <vector>
#include "qlepp2dlib_global.h"

class QLEPP2DLIBSHARED_EXPORT OFFHandler
{

public:
    OFFHandler();
    bool loadOffFile(QString &filepath, std::vector<float> &vertices, std::vector<int> &indices);
    bool saveOffFile(QString &filepath, std::vector<float> &vertices, std::vector<int> &indices) const;

private:
    int m_numVertices;
    int m_numFaces;
    int m_numEdges;
};

#endif // OFFHANDLER_H
