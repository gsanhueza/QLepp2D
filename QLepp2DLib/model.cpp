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

Model::Model() = default;

bool Model::setCPUEngine()
{
    return ModelImpl::getInstance().setCPUEngine();
}
bool Model::setOpenCLEngine()
{
    return ModelImpl::getInstance().setOpenCLEngine();
}

bool Model::loadFile(std::string filepath)
{
    return ModelImpl::getInstance().loadFile(filepath);
}

bool Model::saveFile(std::string filepath)
{
    return ModelImpl::getInstance().saveFile(filepath);
}

std::vector<Vertex>& Model::getVertices()
{
    return ModelImpl::getInstance().getVertices();
}

std::vector<Triangle>& Model::getTriangles()
{
    return ModelImpl::getInstance().getTriangles();
}

bool Model::detectBadTriangles(double angle)
{
    return ModelImpl::getInstance().detectBadTriangles(angle);
}

bool Model::improveTriangulation()
{
    return ModelImpl::getInstance().improveTriangulation();
}
