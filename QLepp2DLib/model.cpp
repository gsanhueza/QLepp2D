/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017-2018 Gabriel Sanhueza <gabriel_8032@hotmail.com>
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

#include <model.h>
#include <model_impl.h>

Model::Model()
    : m_impl(&ModelImpl::getInstance())
{
}

bool Model::setCPUEngine()
{
    return m_impl->setCPUEngine();
}
bool Model::setOpenCLEngine()
{
    return m_impl->setOpenCLEngine();
}

bool Model::loadFile(std::string filepath)
{
    return m_impl->loadFile(filepath);
}

bool Model::saveFile(std::string filepath)
{
    return m_impl->saveFile(filepath);
}

std::vector<Vertex>& Model::getVertices()
{
    return m_impl->getVertices();
}

std::vector<Edge>& Model::getEdges()
{
    return m_impl->getEdges();
}

std::vector<Triangle>& Model::getTriangles()
{
    return m_impl->getTriangles();
}

bool Model::detectBadTriangles(double angle)
{
    return m_impl->detectBadTriangles(angle);
}

bool Model::improveTriangulation()
{
    return m_impl->improveTriangulation();
}
