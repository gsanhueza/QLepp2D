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

#ifndef OFFHANDLER_H
#define OFFHANDLER_H

#include <QString>
#include <vector>
#include "off/offmetadata.h"
#include "structs/triangle.h"
#include "structs/vertex.h"
#include "qlepp2dlib_global.h"

class QLEPP2DLIBSHARED_EXPORT OFFHandler
{

public:
    OFFHandler();
    bool loadOffFile(QString &filepath, OFFMetadata &metadata, std::vector<Vertex> &vertices, std::vector<int> &indices, std::vector<Triangle> &triangles);
    bool saveOffFile(QString &filepath, OFFMetadata &metadata, std::vector<Vertex> &vertices, std::vector<int> &indices) const;

private:
    int m_numVertices;
    int m_numFaces;
    int m_numEdges;
};

#endif // OFFHANDLER_H
