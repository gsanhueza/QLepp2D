#include "model.h"
#include "engine/cpuengine.h"
#include "engine/openclengine.h"

Model::Model()
    : m_offhandler(new OFFHandler)
{
    setEngine(new CPUEngine);
}

Model::Model(AbstractEngine *engine)
    : m_offhandler(new OFFHandler)
{
    setEngine(engine);
}

Model::~Model()
{
}

void Model::setEngine(AbstractEngine *engine)
{
    if (m_engine != nullptr)
    {
        delete m_engine;
    }
    m_engine = engine;
}
bool Model::setCPUEngine()
{
    try
    {
        setEngine(new CPUEngine);
        return true;
    }
    catch (...)
    {
        return false;
    }
}
bool Model::setOpenCLEngine()
{
    try
    {
        setEngine(new OpenCLEngine);
        return true;
    }
    catch (...)
    {
        return false;
    }
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
    return m_engine->detectBadTriangles(angle, m_triangles, m_vertices);
}

bool Model::improveTriangulation()
{
    return m_engine->improveTriangulation(m_triangles, m_vertices, m_indices, m_offmetadata);
}
