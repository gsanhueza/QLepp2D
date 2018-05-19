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
{
    *m_ptr = ModelImpl::getInstance();
}

Model::~Model()
{
    delete m_ptr;
}

ModelImpl& Model::getInstance(void)
{
    return ModelImpl::getInstance();
}

bool Model::setCPUEngine()
{
    return m_ptr->setCPUEngine();
}
bool Model::setOpenCLEngine()
{
    return m_ptr->setOpenCLEngine();
}

bool Model::loadOFF(std::string &filepath)
{
    return m_ptr->loadOFF(filepath);
}

bool Model::saveOFF(std::string &filepath)
{
    return m_ptr->saveOFF(filepath);
}

std::vector<Vertex>& Model::getVertices()
{
    return m_ptr->getVertices();
}

std::vector<Triangle>& Model::getTriangles()
{
    return m_ptr->getTriangles();
}

bool Model::detectBadTriangles(double &angle)
{
    return m_ptr->detectBadTriangles(angle);
}

bool Model::improveTriangulation()
{
    return m_ptr->improveTriangulation();
}
