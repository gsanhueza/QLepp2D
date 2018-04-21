/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017  Gabriel Sanhueza <gabriel_8032@hotmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "model.h"
#include "engine/cpuengine.h"
#include "engine/openclengine.h"

Model& Model::getInstance(void)
{
    static Model instance;
    return instance;
}

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
    return m_offhandler->loadOffFile(filepath, m_offmetadata, m_vertices, m_edges, m_triangles);
}

bool Model::saveOFF(QString &filepath)
{
    return m_offhandler->saveOffFile(filepath, m_offmetadata, m_vertices, m_triangles);
}

std::vector<Vertex>& Model::getVertices()
{
    return m_vertices;
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
    return m_engine->improveTriangulation(m_triangles, m_vertices, m_edges, m_offmetadata);
}
