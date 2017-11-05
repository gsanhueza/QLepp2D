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
