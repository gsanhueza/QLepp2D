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
    qDebug() << "CPU: Processed in" << elapsed << "nanoseconds.";
    return true;
}

int CPUEngine::getTerminalIEdge(int it,
                                std::vector<Triangle> &triangles,
                                std::vector<Vertex> &vertices,
                                std::vector<Edge> &edges) const
{
    QVector<int> triangleHistory;
    int k = 0;                                              // Index of triangleHistory

    triangleHistory.resize(3);
    for (int j(0); j < 3; j++)
    {
        triangleHistory.push_back(-1);
    }
    Triangle t(triangles.at(it));                           // Copy, not reference

    while (true)
    {
        // Add myself to the history.
        triangleHistory[k] = it;

        // Detect longest edge.
        Vertex A, B, C;
        A = vertices.at(t.iv1);
        B = vertices.at(t.iv2);
        C = vertices.at(t.iv3);

        float length_a2 = pow(B.x - C.x, 2) + pow(B.y - C.y, 2) + pow(B.z - C.z, 2);
        float length_b2 = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) + pow(A.z - C.z, 2);
        float length_c2 = pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2);

        int neighbourIT;
        int longestIE;
        Edge longestEdge;

        if (length_a2 >= length_b2 and length_a2 >= length_c2)
        {
            longestEdge = edges.at(t.ie1);
            longestIE = t.ie1;
        }
        else if (length_b2 >= length_a2 and length_b2 >= length_c2)
        {
            longestEdge = edges.at(t.ie2);
            longestIE = t.ie2;
        }
        else
        {
            longestEdge = edges.at(t.ie3);
            longestIE = t.ie3;
        }

        // Detect my neighbour.
        neighbourIT = (longestEdge.ita == it) ? longestEdge.itb : longestEdge.ita;
        qDebug() << "I'm" << it << "and my neighbour is" << neighbourIT;

        if (neighbourIT < 0)
        {
            qDebug() << "Border triangle";
            return longestIE;
        }

        // If I was here before, then I found the final edge of Lepp.
        if (it == triangleHistory.at((k + 1) % 3))          // Equivalent of (k - 2)
        {
            qDebug() << "Found myself, and my longest-edge-shared neighbour";
            return longestIE;
        }

        // Update t to check neighbour
        it = neighbourIT;
        t = triangles.at(neighbourIT);
        k = (k + 1) % 3;

    }
    return -1;
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
    QVector<int> terminalIEdges;

    /* We'll do this in 3 phases:
     * Phase 1: Detect the terminal edges for each bad triangle.
     * Phase 2: Insert new triangle(s) at each terminal edge.
     * Phase 3: Recalculate bad triangles.
     */

    // Phase 1
    for (int i(0); i < static_cast<int>(triangles.size()); i++)
    {
        Triangle &t(triangles.at(i));

        /* Since we need to find the longest edges to get the Lepp, we can
         * just create a protected method std::vector<Edge> getTerminalEdges()
         * that returns a vector of Edges for each t.bad, with a predicted
         * size of #(t.bad).
         * Worst case, just return the vector with size #(triangles), where
         * each position "i" represents the longest edge (position) detected
         * from the triangle "i".
         */
        if (t.bad)
        {
            /* We can just calculate every triangle's lepp in GPU, but only have
             * to calculate the required here in CPU, as there's no need for
             * everyone right now.
             */
            int longestEdge = getTerminalIEdge(i, triangles, vertices, edges);
            terminalIEdges.append(longestEdge); // FIXME How do I avoid duplication?
        }
        t.bad = 0;

    }
    // TODO Delete this
    // Checking terminal edges
    qDebug() << "Found" << terminalIEdges.size() << "terminal edges";
    for (int i : terminalIEdges)
    {
        qDebug() << "Found edge" << i;
    }

    // TODO Phase 2
    // TODO Phase 3

    return true;
}
