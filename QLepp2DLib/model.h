#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "offhandler.h"
#include "openclhandler.h"
#include "vertex.h"
#include "triangle.h"

class Model
{
public:
    Model();
    ~Model();

    bool loadOFF(QString &filepath);
    bool saveOFF(QString &filepath);

    std::vector<Vertex>& getVertices();
    std::vector<int>& getIndices();
    std::vector<Triangle>& getTriangles();

    bool detectBadTriangles(int &angle);

private:
    OFFHandler *m_offhandler;
    OpenCLHandler *m_openclhandler;
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<int> m_indices;
};

#endif // MODEL_H
