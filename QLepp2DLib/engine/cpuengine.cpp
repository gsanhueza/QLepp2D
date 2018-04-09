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
#include <QElapsedTimer>
#include <cmath>
#include "engine/cpuengine.h"
#include "structs/triangle.h"
#include "structs/edge.h"

CPUEngine::CPUEngine() = default;

bool CPUEngine::detectBadTriangles( double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices)
{
    qDebug() << "CPUEngine::detectBadTriangles - angle =" << angle;

    QElapsedTimer timer;
    timer.start();
    for (Triangle &t : triangles)
    {
        Vertex A, B, C;
        A = vertices.at(t.iv1);
        B = vertices.at(t.iv2);
        C = vertices.at(t.iv3);

        float length_a2 = pow(B.x - C.x, 2) + pow(B.y - C.y, 2) + pow(B.z - C.z, 2);
        float length_b2 = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) + pow(A.z - C.z, 2);
        float length_c2 = pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2);

        float length_a = sqrt(length_a2);
        float length_b = sqrt(length_b2);
        float length_c = sqrt(length_c2);

        float angle_opp_a2 = std::acos((length_b2 + length_c2 - length_a2) / (2 * length_b * length_c));
        float angle_opp_b2 = std::acos((length_a2 + length_c2 - length_b2) / (2 * length_a * length_c));
        float angle_opp_c2 = std::acos((length_a2 + length_b2 - length_c2) / (2 * length_a * length_b));

        float rad_angle = angle * M_PI / 180.0;

        t.bad = (angle_opp_a2 < rad_angle or angle_opp_b2 < rad_angle or angle_opp_c2 < rad_angle);
    }
    qint64 elapsed = timer.nsecsElapsed();
    qDebug() << "CPU: Bad triangles detected in" << elapsed << "nanoseconds.";
    return true;
}

void CPUEngine::detectTerminalEdges(std::vector<Edge>& edges,
                                    std::vector<Vertex>& vertices,
                                    std::vector<Triangle>& triangles)
{
    /* We need to check if an edge is the longest edge for both triangles
     * (or "Triangle A" if border edge).
     * Then, we can mark it as a "terminal edge".
     */
    QElapsedTimer timer;
    timer.start();
    for (int ie(0); ie < edges.size(); ie++)
    {
        Edge &e(edges.at(ie));

        Vertex A, B, C;
        float length_a2, length_b2, length_c2;

        // Triangle A
        Triangle &ta(triangles.at(e.ita));
        A = vertices.at(ta.iv1);
        B = vertices.at(ta.iv2);
        C = vertices.at(ta.iv3);

        length_a2 = pow(B.x - C.x, 2) + pow(B.y - C.y, 2) + pow(B.z - C.z, 2);
        length_b2 = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) + pow(A.z - C.z, 2);
        length_c2 = pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2);

        int longestIEA;
        if (length_a2 >= length_b2 and length_a2 >= length_c2)
        {
            longestIEA = ta.ie1;
        }
        else if (length_b2 >= length_a2 and length_b2 >= length_c2)
        {
            longestIEA = ta.ie2;
        }
        else
        {
            longestIEA = ta.ie3;
        }

        /* If edges[ie] is not the longest edge of Triangle A, it won't
         * matter if it's the longest edge of Triangle B, as it can't be a
         * terminal edge.
         */
        if (longestIEA != ie)
        {
            e.isTerminalEdge = 0;
            continue;
        }
        else if (e.itb < 0)                                 // "A" might be a border triangle with the longest edge
        {
            e.isTerminalEdge = 1;
            continue;
        }

        // Triangle B
        Triangle &tb(triangles.at(e.itb));
        A = vertices.at(tb.iv1);
        B = vertices.at(tb.iv2);
        C = vertices.at(tb.iv3);

        length_a2 = pow(B.x - C.x, 2) + pow(B.y - C.y, 2) + pow(B.z - C.z, 2);
        length_b2 = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) + pow(A.z - C.z, 2);
        length_c2 = pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2);

        int longestIEB;
        if (length_a2 >= length_b2 and length_a2 >= length_c2)
        {
            longestIEB = tb.ie1;
        }
        else if (length_b2 >= length_a2 and length_b2 >= length_c2)
        {
            longestIEB = tb.ie2;
        }
        else
        {
            longestIEB = tb.ie3;
        }

        /* If edges[ie] is not the longest edge of Triangle B, it won't
         * matter if it's the longest edge of Triangle A, as it can't be a
         * terminal edge.
         */
        if (longestIEB != ie)
        {
            e.isTerminalEdge = 0;
            continue;
        }

        /* Final check, if the longest edges of each triangle are the same as
         * "this" edge, we have our terminal edge.
         */
        if (longestIEA == longestIEB and longestIEB == ie)
        {
            e.isTerminalEdge = 1;
        }
        else
        {
            e.isTerminalEdge = 0;
        }
    }

    qint64 elapsed = timer.nsecsElapsed();
    qDebug() << "CPU: Terminal Edges detected in" << elapsed << "nanoseconds.";
}

bool CPUEngine::improveTriangulation(   std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        std::vector<Edge> &edges,
                                        OFFMetadata &metadata)
{
    // TODO
    // Each insertion does:
    // +1 to metadata.vertices
    // +2 to metadata.indices
    // +3 to metadata.edges
    qDebug() << "CPUEngine::improveTriangulation";

    /* We'll do this in 3 phases:
     * Phase 1: Detect terminal edges.
     * Phase 2: Insert new triangle(s) at each terminal edge if needed.
     * Phase 3: Recalculate bad triangles.
     */

    for (Triangle &t : triangles)
    {
        t.bad = 0;
    }

    // Phase 1
    detectTerminalEdges(edges, vertices, triangles);

    // TODO Delete this
    // Checking terminal edges
    for (int i(0); i < edges.size(); i++)
    {
        qDebug() << "Edge" << i << ": isTerminalEdge = " << edges.at(i).isTerminalEdge;
    }

    // TODO Phase 2
    // TODO Phase 3

    return true;
}
