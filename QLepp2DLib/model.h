#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "offhandler.h"
#include "offmetadata.h"
#include "abstractengine.h"
#include "vertex.h"
#include "triangle.h"

class Model
{
public:
    Model();
    Model(AbstractEngine *engine);
    ~Model();

    bool loadOFF(QString &filepath);
    bool saveOFF(QString &filepath);

    std::vector<Vertex>& getVertices();
    std::vector<int>& getIndices();
    std::vector<Triangle>& getTriangles();

    bool detectBadTriangles(double &angle);
    bool improveTriangulation();

private:
    OFFHandler *m_offhandler;
    OFFMetadata m_offmetadata;
    AbstractEngine *m_engine;
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<int> m_indices;
};

#endif // MODEL_H
