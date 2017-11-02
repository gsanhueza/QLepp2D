#include "model.h"
#include "cpuengine.h"
#include "openclengine.h"

Model::Model()
    : m_offhandler(new OFFHandler),
      m_engine(new OpenCLEngine)
{
}

Model::Model(AbstractEngine *engine)
    : m_offhandler(new OFFHandler)
{
    m_engine = engine;
}

Model::~Model()
{
}

bool Model::loadOFF(QString &filepath)
{
    return m_offhandler->loadOffFile(filepath, m_offmetadata, m_vertices, m_indices, m_triangles);
}

bool Model::saveOFF(QString &filepath)
{
    return m_offhandler->saveOffFile(filepath, m_offmetadata, m_vertices, m_indices);
}

std::vector<Vertex>& Model::getVertices()
{
    return m_vertices;
}

std::vector<int>& Model::getIndices()
{
    return m_indices;
}

std::vector<Triangle>& Model::getTriangles()
{
    return m_triangles;
}

bool Model::detectBadTriangles(double &angle)
{
    return m_engine->detectBadTriangles(angle, m_triangles, m_vertices, m_indices);
}

bool Model::improveTriangulation()
{
    return m_engine->improveTriangulation(m_triangles, m_vertices, m_indices, m_offmetadata);
}
