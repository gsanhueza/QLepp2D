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

#include <QDebug>
#include <QElapsedTimer>
#include <cmath>
#include "engine/cpuengine.h"
#include "structs/triangle.h"
#include "structs/edge.h"

CPUEngine::CPUEngine() = default;

bool CPUEngine::detectBadTriangles(double &angle,
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
    qDebug() << "CPU: Bad Triangles detected in" << elapsed << "nanoseconds.";
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

        if (length_a2 > length_b2 and length_a2 > length_c2)
        {
            longestEdge = edges.at(t.ie1);
            longestIE = t.ie1;
        }
        else if (length_b2 > length_a2 and length_b2 > length_c2)
        {
            longestEdge = edges.at(t.ie2);
            longestIE = t.ie2;
        }
        else // Unique longest Edge guaranteed here
        {
            longestEdge = edges.at(t.ie3);
            longestIE = t.ie3;
        }

        // Detect my neighbour.
        neighbourIT = (longestEdge.ita == it) ? longestEdge.itb : longestEdge.ita;

        // Border triangle
        if (neighbourIT < 0)
        {
            edges.at(longestIE).isBorderEdge = 1;
            return longestIE;
        }

        // If I was here before, then I found the final edge of Lepp.
        if (it == triangleHistory.at((k + 1) % 3))          // Equivalent of (k - 2)
        {
            return longestIE;
        }

        // Update t to check neighbour
        it = neighbourIT;
        t = triangles.at(neighbourIT);
        k = (k + 1) % 3;

    }
    return -1;
}

void CPUEngine::detectTerminalEdges(std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices,
                                    std::vector<Edge> &edges)
{
    QElapsedTimer timer;
    timer.start();

    for (int i(0); i < static_cast<int>(triangles.size()); i++)
    {
        Triangle &t(triangles.at(i));

        /* Since we need to find the longest edges to get the Lepp, we can
         * just create a protected method "int getTerminalIEdge(...)" that returns
         * the index of the edge that is a terminal edge.
         *
         * From here, we can update the "edges" vector, and each of these edges
         * will know if it's a terminal edge that has to be modified or not.
         */
        if (t.bad)
        {
            /* We can just calculate every triangle's lepp in GPU, but only have
             * to calculate the required here in CPU, as there's no need for
             * everyone right now.
             */
            int longestIEdge = getTerminalIEdge(i, triangles, vertices, edges);
            edges.at(longestIEdge).isTerminalEdge = 1;
        }
    }

    qint64 elapsed = timer.nsecsElapsed();
    qDebug() << "CPU: Terminal Edges detected in" << elapsed << "nanoseconds.";
}

Vertex centroidOf(int iva,
                  int ivb,
                  int ivc,
                  int ivd,
                  std::vector<Vertex> &vertices)
{
    Vertex centroid;
    centroid.x = (vertices.at(iva).x +
                  vertices.at(ivb).x +
                  vertices.at(ivc).x +
                  vertices.at(ivd).x) / 4.0;
    centroid.y = (vertices.at(iva).y +
                  vertices.at(ivb).y +
                  vertices.at(ivc).y +
                  vertices.at(ivd).y) / 4.0;
    centroid.z = (vertices.at(iva).z +
                  vertices.at(ivb).z +
                  vertices.at(ivc).z +
                  vertices.at(ivd).z) / 4.0;

    return centroid;
}

void insertCentroid(int iedge,
                    std::vector<Triangle> &triangles,
                    std::vector<Vertex> &vertices,
                    std::vector<Edge> &edges)
{
    /* This is the difficult part of the project.
     * The algorithm is divided in 7 "phases", that will be documented here.
     *
     * Phase 1: Detect the 4 vertices in the triangles marked by edges[iedge],
     * get the centroid and insert it in the "vertices" vector.
     * Phase 2: Detect the 4 non-shared edges of the 2 triangles and create
     * 4 more edges.
     * Phase 3: Create 4 triangles with the 4 vertices plus the centroid.
     * (Note: These triangles have incomplete data at the moment, they only know
     * their vertices, not the indices of their edges.)
     * Phase 4: Recycle 2 positions and create 2 more positions in the "triangles"
     * vector, so triangles are now inserted. Remember their positions!
     * Phase 5: For each 2 neighbour triangles (from the new triangles that we
     * created in Phase 3), assign one of the created edges and update the
     * indices of the triangles for that edge. (We repeat this 4 times, as we
     * need to update the 4 new edges with this information.)
     * Phase 6: Recycle 1 position and create 3 more positions in the "edges"
     * vector, so edges are now inserted, and with complete information.
     * Phase 7: For each of the new triangles (that are already in "triangles"
     * vector), update their indices to edges (by reference).
     */

    // Note: These are copies (not references) because we'll replace them at the end.
    Edge oldE(edges.at(iedge));
    Triangle oldTA(triangles.at(oldE.ita));
    Triangle oldTB(triangles.at(oldE.itb));

    // Phase 1
    // Get indices to vertices, so we can easily check for duplicates.

    /* Note: If we want to create correct triangles (and not a broken line),
     * we need to use a combination of vertices "in the shared edge" and "not in
     * the shared edge".
     * A triangle is correct if it follows the SNC or NSC pattern, where:
     *   S = Vertex that is in the shared edge.
     *   N = Vertex that isn't in the shared edge.
     *   C = Centroid.
     *
     * Thus, we'll create a triangle with Pattern[i], Pattern[(i + 1) % 4], Centroid.
     */

    // NSC pattern will be used arbitrarily here.
    QVector<int> iVertexPattern;
    iVertexPattern.push_back(-1);
    iVertexPattern.push_back(oldE.iv1);
    iVertexPattern.push_back(-1);
    iVertexPattern.push_back(oldE.iv2);

    int i = 0;
    for (int iv : {oldTA.iv1, oldTA.iv2, oldTA.iv3, oldTB.iv1, oldTB.iv2, oldTB.iv3})
    {
        if (not iVertexPattern.contains(iv))
        {
            iVertexPattern[i] = iv;
            i = 2;
        }
    }

    // Create our centroid
    Vertex centroid = centroidOf(iVertexPattern.at(0),
                                 iVertexPattern.at(1),
                                 iVertexPattern.at(2),
                                 iVertexPattern.at(3),
                                 vertices);
    vertices.push_back(centroid);
    int iCentroid = vertices.size() - 1; // Index of new Vertex

    // Phase 2
    // Detect outer edges (the ones we don't share)
    QVector<int> nonSharedIEdges;
    for (int ie : {oldTA.ie1, oldTA.ie2, oldTA.ie3, oldTB.ie1, oldTB.ie2, oldTB.ie3})
    {
        if (ie != iedge)
        {
            nonSharedIEdges.append(ie);
        }
    }

    // Create new Edges
    Edge EA, EB, EC, ED;

    // Phase 3
    // Create new Triangles
    Triangle TA, TB, TC, TD;

    TA.iv1 = iVertexPattern.at(0);
    TA.iv2 = iVertexPattern.at(1);
    TA.iv3 = iCentroid;
    TA.ie1 = TA.ie2 = TA.ie3 = -1;
    TA.bad = 0;

    TB.iv1 = iVertexPattern.at(1);
    TB.iv2 = iVertexPattern.at(2);
    TB.iv3 = iCentroid;
    TB.ie1 = TB.ie2 = TB.ie3 = -1;
    TB.bad = 0;

    TC.iv1 = iVertexPattern.at(2);
    TC.iv2 = iVertexPattern.at(3);
    TC.iv3 = iCentroid;
    TC.ie1 = TC.ie2 = TC.ie3 = -1;
    TC.bad = 0;

    TD.iv1 = iVertexPattern.at(3);
    TD.iv2 = iVertexPattern.at(0);
    TD.iv3 = iCentroid;
    TD.ie1 = TD.ie2 = TD.ie3 = -1;
    TD.bad = 0;

    // Phase 4
    int ITA, ITB, ITC, ITD; // Indices of triangles (A and B are recycled)
    ITA = oldE.ita;
    ITB = oldE.itb;
    triangles.at(ITA) = TA;
    triangles.at(ITB) = TB;
    triangles.push_back(TC);
    ITC = triangles.size() - 1;
    triangles.push_back(TD);
    ITD = triangles.size() - 1;

    // Phase 5
    /* We'll work with our new triangles. We'll take two of them that share one
     * of the new edges and assign them to one of our new edges.
     * (If we study how we decided the order of the vertices in "vertexPattern",
     * we'll notice that our new triangles are always neighbours.
     * We cannot know if they're CCW or CW neighbours, but the triangle n and the
     * triangle n + 1 will always share one of the new edges.)
     * We'll update the information of this edge, but the triangles will keep
     * incomplete data at the moment, because we need to insert these new edges first.
     */
    EA.ita = ITA;
    EA.itb = ITB;
    EA.iv1 = std::min(TA.iv2, TA.iv3);
    EA.iv2 = std::max(TA.iv2, TA.iv3);
    EA.isTerminalEdge = EA.isBorderEdge = 0;

    EB.ita = ITB;
    EB.itb = ITC;
    EB.iv1 = std::min(TB.iv2, TB.iv3);
    EB.iv2 = std::max(TB.iv2, TB.iv3);
    EB.isTerminalEdge = EB.isBorderEdge = 0;

    EC.ita = ITC;
    EC.itb = ITD;
    EC.iv1 = std::min(TC.iv2, TC.iv3);
    EC.iv2 = std::max(TC.iv2, TC.iv3);
    EC.isTerminalEdge = EC.isBorderEdge = 0;

    ED.ita = ITD;
    ED.itb = ITA;
    ED.iv1 = std::min(TD.iv2, TD.iv3);
    ED.iv2 = std::max(TD.iv2, TD.iv3);
    ED.isTerminalEdge = ED.isBorderEdge = 0;

    /* We still have to update information of triangles for the nonSharedEdges,
     * so we'll change the index of the ita/itb that had the old triangle,
     * and update it with our new triangles.
     */
    for (int ie : nonSharedIEdges)
    {
        Edge &e(edges.at(ie));
        // Triangle A
        if (e.iv1 == std::min(triangles.at(ITA).iv1, triangles.at(ITA).iv2) and e.iv2 == std::max(triangles.at(ITA).iv1, triangles.at(ITA).iv2))
        {
            triangles.at(ITA).ie3 = ie;
            if (e.ita == oldE.ita or e.ita == oldE.itb)
            {
                e.ita = ITA;
            }
            else if (e.itb == oldE.ita or e.itb == oldE.itb)
            {
                e.itb = ITA;
            }
        }

        // Triangle B
        if (e.iv1 == std::min(triangles.at(ITB).iv1, triangles.at(ITB).iv2) and e.iv2 == std::max(triangles.at(ITB).iv1, triangles.at(ITB).iv2))
        {
            triangles.at(ITB).ie3 = ie;
            if (e.ita == oldE.ita or e.ita == oldE.itb)
            {
                e.ita = ITB;
            }
            else if (e.itb == oldE.ita or e.itb == oldE.itb)
            {
                e.itb = ITB;
            }
        }

        // Triangle C
        if (e.iv1 == std::min(triangles.at(ITC).iv1, triangles.at(ITC).iv2) and e.iv2 == std::max(triangles.at(ITC).iv1, triangles.at(ITC).iv2))
        {
            triangles.at(ITC).ie3 = ie;
            if (e.ita == oldE.ita or e.ita == oldE.itb)
            {
                e.ita = ITC;
            }
            else if (e.itb == oldE.ita or e.itb == oldE.itb)
            {
                e.itb = ITC;
            }
        }

        // Triangle D
        if (e.iv1 == std::min(triangles.at(ITD).iv1, triangles.at(ITD).iv2) and e.iv2 == std::max(triangles.at(ITD).iv1, triangles.at(ITD).iv2))
        {
            triangles.at(ITD).ie3 = ie;
            if (e.ita == oldE.ita or e.ita == oldE.itb)
            {
                e.ita = ITD;
            }
            else if (e.itb == oldE.ita or e.itb == oldE.itb)
            {
                e.itb = ITD;
            }
        }

    }

    // Phase 6
    int IEA, IEB, IEC, IED; // Indices of edges (A is recycled)
    IEA = iedge;
    edges.at(IEA) = EA;
    edges.push_back(EB);
    IEB = edges.size() - 1;
    edges.push_back(EC);
    IEC = edges.size() - 1;
    edges.push_back(ED);
    IED = edges.size() - 1;

    // TODO Phase 7
    /* Available edges are EA, EB, EC, ED.
     * Note: Because we deliberately put our centroid in iv3, we know that ie3
     * is one of the "nonSharedEdges".
     *
     * Example of detection:
     *   TA.ie1 = Index of Edge whose iv1 and iv2 are TA.iv2 and TA.iv3
     */

    QVector<int> availableIEdges = {IEA, IEB, IEC, IED};

    for (int ie : availableIEdges)
    {
        Edge &e(edges.at(ie));

        // Triangle A (ie1)
        if (e.iv1 == std::min(triangles.at(ITA).iv2, triangles.at(ITA).iv3) and
            e.iv2 == std::max(triangles.at(ITA).iv2, triangles.at(ITA).iv3))
        {
            triangles.at(ITA).ie1 = ie;
        }
        // Triangle A (ie2)
        if (e.iv1 == std::min(triangles.at(ITA).iv1, triangles.at(ITA).iv3) and
            e.iv2 == std::max(triangles.at(ITA).iv1, triangles.at(ITA).iv3))
        {
            triangles.at(ITA).ie2 = ie;
        }

        // Triangle B (ie1)
        if (e.iv1 == std::min(triangles.at(ITB).iv2, triangles.at(ITB).iv3) and
            e.iv2 == std::max(triangles.at(ITB).iv2, triangles.at(ITB).iv3))
        {
            triangles.at(ITB).ie1 = ie;
        }
        // Triangle B (ie2)
        if (e.iv1 == std::min(triangles.at(ITB).iv1, triangles.at(ITB).iv3) and
            e.iv2 == std::max(triangles.at(ITB).iv1, triangles.at(ITB).iv3))
        {
            triangles.at(ITB).ie2 = ie;
        }

        // Triangle C (ie1)
        if (e.iv1 == std::min(triangles.at(ITC).iv2, triangles.at(ITC).iv3) and
            e.iv2 == std::max(triangles.at(ITC).iv2, triangles.at(ITC).iv3))
        {
            triangles.at(ITC).ie1 = ie;
        }
        // Triangle C (ie2)
        if (e.iv1 == std::min(triangles.at(ITC).iv1, triangles.at(ITC).iv3) and
            e.iv2 == std::max(triangles.at(ITC).iv1, triangles.at(ITC).iv3))
        {
            triangles.at(ITC).ie2 = ie;
        }

        // Triangle D (ie1)
        if (e.iv1 == std::min(triangles.at(ITD).iv2, triangles.at(ITD).iv3) and
            e.iv2 == std::max(triangles.at(ITD).iv2, triangles.at(ITD).iv3))
        {
            triangles.at(ITD).ie1 = ie;
        }
        // Triangle D (ie2)
        if (e.iv1 == std::min(triangles.at(ITD).iv1, triangles.at(ITD).iv3) and
            e.iv2 == std::max(triangles.at(ITD).iv1, triangles.at(ITD).iv3))
        {
            triangles.at(ITD).ie2 = ie;
        }
    }
}

void insertCentroids(std::vector<Triangle> &triangles,
                     std::vector<Vertex> &vertices,
                     std::vector<Edge> &edges)
{
    for (unsigned int ie(0); ie < edges.size(); ie++)
    {
        Edge &e(edges.at(ie));
        if (e.isTerminalEdge and not e.isBorderEdge)
        {
            insertCentroid(ie, triangles, vertices, edges);
        }
    }
}

bool CPUEngine::improveTriangulation(std::vector<Triangle> &triangles,
                                     std::vector<Vertex> &vertices,
                                     std::vector<Edge> &edges,
                                     OFFMetadata &metadata)
{
    // TODO
    // Each insertion does:
    // +1 to metadata.vertices (vertices.size())
    // +2 to metadata.faces (triangles.size())
    // +3 to metadata.edges (edges.size())
    qDebug() << "CPUEngine::improveTriangulation";

    /* We'll do this in 3 phases:
     * Phase 1: Detect the terminal edges for each bad triangle.
     * Phase 2: Insert new triangle(s) at each terminal edge.
     * Phase 3: Recalculate bad triangles.
     */

    bool nonBorderTerminalEdgesRemaining = false;
    do
    {
        // Phase 1
        detectTerminalEdges(triangles, vertices, edges);

        // Phase 2
        insertCentroids(triangles, vertices, edges);

        // TODO Phase 3

        // TODO Delete this
        for (Triangle &t : triangles)
        {
            t.bad = 0;
        }
    }
    while (nonBorderTerminalEdgesRemaining);

    return true;
}
