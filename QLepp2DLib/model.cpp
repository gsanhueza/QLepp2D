#include "model.h"

Model::Model()
    : m_offhandler(new OFFHandler)
{
}

Model::~Model()
{
    delete m_offhandler;
}

bool Model::loadOFF(QString filepath) const
{
    return m_offhandler->loadOffFile(filepath);
}

bool Model::saveOFF(QString filepath) const
{
    return m_offhandler->saveOffFile(filepath);
}
