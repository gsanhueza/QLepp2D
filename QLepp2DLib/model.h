#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "offhandler.h"
#include "vertex.h"
#include "triangle.h"

class Model
{
public:
    Model();
    ~Model();

    bool loadOFF(QString &filepath);
    bool saveOFF(QString &filepath);
    std::vector<Vertex> getVertices() const;
    std::vector<int> getIndices() const;
    std::vector<Triangle> getTriangles() const;

private:
    OFFHandler *m_offhandler;
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<int> m_indices;
};

#endif // MODEL_H
