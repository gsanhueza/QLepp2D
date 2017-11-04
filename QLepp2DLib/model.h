#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "off/offhandler.h"
#include "off/offmetadata.h"

#include "structs/vertex.h"
#include "structs/triangle.h"

#include "engine/abstractengine.h"

class Model
{
public:
    Model();
    Model(AbstractEngine *engine);
    ~Model();

    void setEngine(AbstractEngine *engine);
    bool setCPUEngine();
    bool setOpenCLEngine();

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
    AbstractEngine *m_engine = nullptr;
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<int> m_indices;
};

#endif // MODEL_H
