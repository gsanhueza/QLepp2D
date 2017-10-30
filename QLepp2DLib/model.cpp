#include "model.h"

Model::Model()
    : m_offhandler(new OFFHandler)
{
}

Model::~Model()
{
    delete m_offhandler;
}

bool Model::loadOFF(QString &filepath)
{
    return m_offhandler->loadOffFile(filepath, m_vertices, m_indices);
}

bool Model::saveOFF(QString &filepath)
{
    return m_offhandler->saveOffFile(filepath, m_vertices, m_indices);
}
