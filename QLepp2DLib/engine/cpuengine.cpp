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

#include <QDebug>
#include <cmath>
#include "engine/cpuengine.h"
#include "structs/triangle.h"
#include "structs/edge.h"

CPUEngine::CPUEngine()
{
}

bool CPUEngine::detectBadTriangles( double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices)
{
    qDebug() << "CPUEngine::detectBadTriangles - angle =" << angle;
    for (Triangle &t : triangles)
    {
        // FIXME Read edge data from Triangle struct
        Edge A, B, C;
        A.a = vertices.at(t.i1);
        A.b = vertices.at(t.i2);
        B.a = vertices.at(t.i2);
        B.b = vertices.at(t.i3);
        C.a = vertices.at(t.i3);
        C.b = vertices.at(t.i1);

        float length_a2 = pow(A.a.x - A.b.x, 2) + pow(A.a.y - A.b.y, 2) + pow(A.a.z - A.b.z, 2);
        float length_b2 = pow(B.a.x - B.b.x, 2) + pow(B.a.y - B.b.y, 2) + pow(B.a.z - B.b.z, 2);
        float length_c2 = pow(C.a.x - C.b.x, 2) + pow(C.a.y - C.b.y, 2) + pow(C.a.z - C.b.z, 2);

        float length_a = sqrt(length_a2);
        float length_b = sqrt(length_b2);
        float length_c = sqrt(length_c2);

        float angle_opp_a2 = std::acos((length_b2 + length_c2 - length_a2) / (2 * length_b * length_c));
        float angle_opp_b2 = std::acos((length_a2 + length_c2 - length_b2) / (2 * length_a * length_c));
        float angle_opp_c2 = std::acos((length_a2 + length_b2 - length_c2) / (2 * length_a * length_b));

        float rad_angle = angle * M_PI / 180.0;

        t.bad = (angle_opp_a2 < rad_angle or angle_opp_b2 < rad_angle or angle_opp_c2 < rad_angle);
    }
    return true;
}

bool CPUEngine::improveTriangulation(   std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata)
{
    // TODO
    // Each insertion does:
    // +1 to metadata.vertices
    // +2 to metadata.indices
    // +3 to metadata.edges
    qDebug() << "CPUEngine::improveTriangulation";
    for (Triangle &t : triangles)
    {
        t.bad = 0;
    }
    return true;
}
