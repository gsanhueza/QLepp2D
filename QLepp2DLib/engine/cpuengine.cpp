/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017-2019 Gabriel Sanhueza <gabriel_8032@hotmail.com>
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
#include <engine/cpuengine.h>
#include <structs/triangle.h>
#include <structs/edge.h>

CPUEngine::CPUEngine()
{
    m_angle = 0;
}

bool CPUEngine::detectBadTriangles(float angle,
                                   std::vector<Vertex> &vertices,
                                   std::vector<Triangle> &triangles)
{
    qDebug() << "(CPU) Angle :" << angle;
    m_angle = angle;

    QElapsedTimer timer;
    timer.start();

    for (Triangle &t : triangles)
    {
        Vertex A, B, C;
        A = vertices.at(t.iv1);
        B = vertices.at(t.iv2);
        C = vertices.at(t.iv3);

        float length_A = pow(B.x - C.x, 2) + pow(B.y - C.y, 2) + pow(B.z - C.z, 2);
        float length_B = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) + pow(A.z - C.z, 2);
        float length_C = pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2);

        float length_a = sqrt(length_A);
        float length_b = sqrt(length_B);
        float length_c = sqrt(length_C);

        float angle_opp_A = std::acos((length_B + length_C - length_A)
                                      / (2 * length_b * length_c));
        float angle_opp_B = std::acos((length_A + length_C - length_B)
                                      / (2 * length_a * length_c));
        float angle_opp_C = std::acos((length_A + length_B - length_C)
                                      / (2 * length_a * length_b));

        float rad_angle = angle * static_cast<float>(M_PI) / 180.0f;

        t.bad = (angle_opp_A < rad_angle or
                 angle_opp_B < rad_angle or
                 angle_opp_C < rad_angle);
    }
    qint64 elapsed = timer.nsecsElapsed();
    qInfo() << "___";
    qInfo() << "(CPU) DBT_F :" << elapsed << "nanoseconds";

    // Only to check ratio of bad/total triangles (won't be in benchmark!)
    int badCount{0};
    for (Triangle &t : triangles)
    {
        badCount += t.bad;
    }
    qDebug() << "Ratio:" << static_cast<float>(badCount) / triangles.size();

    return true;
}

bool CPUEngine::improveTriangulation(std::vector<Vertex> &vertices,
                                     std::vector<Edge> &edges,
                                     std::vector<Triangle> &triangles)
{
    /* Relevant information: Each insertion does
     *   +1 to vertices.size()
     *   +2 to triangles.size()
     *   +3 to edges.size()
     */

    qDebug() << "CPUEngine::improveTriangulation";

    /* We'll do this in 3 phases:
     * Phase 1: Detect the terminal edges for each bad triangle.
     * Phase 2: Insert new triangle(s) at each terminal edge.
     * Phase 3: Recalculate bad triangles.
     */

    try
    {
        // Phase 1
        bool nonBTERemaining = false; // Flag that shows if we still have Non-border Terminal Edges.
        detectTerminalEdges(vertices, edges, triangles, nonBTERemaining);

        if (not nonBTERemaining)
        {
            return true;
        }

        // Phase 2
        insertCentroids(vertices, edges, triangles);

        // Phase 3
        detectBadTriangles(m_angle, vertices, triangles);

        return true;
    }
    catch (std::exception &e)
    {
        qWarning() << e.what();
        return false;
    }
    catch (...)
    {
        qWarning() << "Unknown error in CPUEngine::improveTriangulation";
        return false;
    }
}

void CPUEngine::detectTerminalEdges(std::vector<Vertex> &vertices,
                                    std::vector<Edge> &edges,
                                    std::vector<Triangle> &triangles,
                                    bool &flag)
{
    QElapsedTimer timer;
    timer.start();

    for (int i(0); i < static_cast<int>(triangles.size()); i++)
    {
        Triangle &t(triangles.at(i));

        /* Since we need to find the longest edges to get the Lepp, we can
         * just create a protected method "int getTerminalIEdge()" that returns
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
            int longestIEdge = getTerminalIEdge(i, vertices, edges, triangles, flag);
            edges.at(longestIEdge).isTE = 1;
        }
    }

    qint64 elapsed = timer.nsecsElapsed();
    qInfo() << "(CPU) DTE_F :" << elapsed << "nanoseconds";
}

void CPUEngine::insertCentroids(std::vector<Vertex> &vertices,
                                std::vector<Edge> &edges,
                                std::vector<Triangle> &triangles)
{
    QElapsedTimer timer;
    timer.start();

    for (unsigned int ie(0); ie < edges.size(); ie++)
    {
        Edge &e(edges.at(ie));
        // If e.itb == -1, it's a border edge, so we won't insert a centroid
        if (e.isTE and e.itb != -1)
        {
            insertCentroid(ie, vertices, edges, triangles);
        }
    }

    qint64 elapsed = timer.nsecsElapsed();
    qInfo() << "(CPU)  IC_F :" << elapsed << "nanoseconds";
}

int CPUEngine::getTerminalIEdge(int it,
                                std::vector<Vertex> &vertices,
                                std::vector<Edge> &edges,
                                std::vector<Triangle> &triangles,
                                bool &flag) const
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

        float length_A = pow(B.x - C.x, 2) + pow(B.y - C.y, 2) + pow(B.z - C.z, 2);
        float length_B = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) + pow(A.z - C.z, 2);
        float length_C = pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2);

        int neighbourIT;
        int longestIE;
        Edge longestEdge;

        if (length_A > length_B and length_A > length_C)
        {
            longestEdge = edges.at(t.ie1);
            longestIE = t.ie1;
        }
        else if (length_B > length_A and length_B > length_C)
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
            return longestIE;
        }

        // If I was here before, then I found the final edge of Lepp.
        if (it == triangleHistory.at((k + 1) % 3))          // Equivalent of (k - 2)
        {
            flag = true;
            return longestIE;
        }

        // Update t to check neighbour
        it = neighbourIT;
        t = triangles.at(neighbourIT);
        k = (k + 1) % 3;

    }
    return -1;
}

Vertex CPUEngine::centroidOf(int iva,
                             int ivb,
                             int ivc,
                             int ivd,
                             std::vector<Vertex> &vertices)
{
    Vertex centroid;
    centroid.x = (vertices.at(iva).x +
                  vertices.at(ivb).x +
                  vertices.at(ivc).x +
                  vertices.at(ivd).x) / 4.0f;
    centroid.y = (vertices.at(iva).y +
                  vertices.at(ivb).y +
                  vertices.at(ivc).y +
                  vertices.at(ivd).y) / 4.0f;
    centroid.z = (vertices.at(iva).z +
                  vertices.at(ivb).z +
                  vertices.at(ivc).z +
                  vertices.at(ivd).z) / 4.0f;

    return centroid;
}

void CPUEngine::insertCentroid(int iedge,
                               std::vector<Vertex> &vertices,
                               std::vector<Edge> &edges,
                               std::vector<Triangle> &triangles)
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

    // Check if our future triangles will be CCW with the "shoelace formula" (Gauss area).
    // Invert iVertexPattern[1] and [3] if they're not CCW.

    /* Note: Since we'll create our 4 new triangles in the same order, they will
     * be all CCW or all CW. If we just invert 2 non-adjacent vertices from
     * the pattern, the orientation of every triangle will change.
     */
    {
        float area(0.0f);
        int n(3);

        int j(n - 1);
        for (int i(0); i < n; i++)
        {
            area += (vertices.at(iVertexPattern.at(j)).x + vertices.at(iVertexPattern.at(i)).x) *
                    (vertices.at(iVertexPattern.at(j)).y - vertices.at(iVertexPattern.at(i)).y);
            j = i;  // j is previous vertex to i
        }

        // Area < 0 ==> CCW
        if (area > 0)
        {
            int tmp(iVertexPattern.at(1));
            iVertexPattern[1] = iVertexPattern.at(3);
            iVertexPattern[3] = tmp;
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
    QVector<Edge> newEdges;
    for (int ie(0); ie < 4; ie++)
    {
        Edge e;
        e.ita = e.itb = e.iv1 = e.iv2 = -1;
        e.isTE = 0;

        newEdges.append(e);
    }

    // Phase 3
    // Create new Triangles
    QVector<Triangle> newTriangles;
    for (int it(0); it < 4; it++)
    {
        // We'll create triangles with iVertexPattern[i], iVertexPattern[(i + 1) % 4], Centroid.
        Triangle t;
        t.iv1 = iVertexPattern.at(it);
        t.iv2 = iVertexPattern.at((it + 1) % 4);
        t.iv3 = iCentroid;
        t.ie1 = t.ie2 = t.ie3 = -1;
        t.bad = 0;

        newTriangles.append(t);
    }

    // Phase 4
    QVector<int> newITriangles; // Indices of triangles (A and B are recycled)
    newITriangles.append(oldE.ita);
    newITriangles.append(oldE.itb);
    triangles.at(newITriangles.at(0)) = newTriangles.at(0);
    triangles.at(newITriangles.at(1)) = newTriangles.at(1);
    triangles.push_back(newTriangles.at(2));
    newITriangles.append(triangles.size() - 1);
    triangles.push_back(newTriangles.at(3));
    newITriangles.append(triangles.size() - 1);

    // Phase 5
    /* We'll work with our new triangles. We'll take two of them that share one
     * of the new edges and assign them to one of our new edges.
     * (If we study how we decided the order of the vertices in "vertexPattern",
     * we'll notice that our new triangles are always neighbours.
     * We forced them to be CCW neighbours, so the triangle n and the
     * triangle n + 1 will always share one of the new edges.)
     * We'll update the information of this edge, but the triangles will keep
     * incomplete data at the moment, because we need to insert these new edges first.
     */

    for (int i(0); i < 4; i++)
    {
        newEdges[i].ita = newITriangles.at(i);
        newEdges[i].itb = newITriangles.at((i + 1) % 4);
        newEdges[i].iv1 = std::min(newTriangles.at(i).iv2, newTriangles.at(i).iv3);
        newEdges[i].iv2 = std::max(newTriangles.at(i).iv2, newTriangles.at(i).iv3);
    }

    /* We still have to update information of triangles for the nonSharedEdges,
     * so we'll change the index of the ita/itb that had the old triangle,
     * and update it with our new triangles.
     */
    for (int ie : nonSharedIEdges)
    {
        Edge &e(edges.at(ie));

        for (int it(0); it < 4; it++)
        {
            if (e.iv1 == std::min(triangles.at(newITriangles.at(it)).iv1, triangles.at(newITriangles.at(it)).iv2) and
                e.iv2 == std::max(triangles.at(newITriangles.at(it)).iv1, triangles.at(newITriangles.at(it)).iv2))
            {
                triangles.at(newITriangles.at(it)).ie3 = ie;
                if (e.ita == oldE.ita or e.ita == oldE.itb)
                {
                    e.ita = newITriangles.at(it);
                }
                else if (e.itb == oldE.ita or e.itb == oldE.itb)
                {
                    e.itb = newITriangles.at(it);
                }
            }
        }
    }

    // Phase 6
    QVector<int> newIEdges;
    newIEdges.append(iedge);
    edges.at(newIEdges.at(0)) = newEdges.at(0);
    edges.push_back(newEdges.at(1));
    newIEdges.append(edges.size() - 1);
    edges.push_back(newEdges.at(2));
    newIEdges.append(edges.size() - 1);
    edges.push_back(newEdges.at(3));
    newIEdges.append(edges.size() - 1);

    // Phase 7
    /* Available edges for this phase are the new edges.
     * Note: Because we deliberately put our centroid in iv3, we know that ie3
     * is one of the "nonSharedEdges".
     *
     * Example of detection:
     *   TA.ie1 = Index of Edge whose iv1 and iv2 are TA.iv2 and TA.iv3
     */

    for (int ie : newIEdges)
    {
        Edge &e(edges.at(ie));

        for (int it(0); it < 4; it++)
        {
            // Triangle "it" (ie1)
            if (e.iv1 == std::min(triangles.at(newITriangles.at(it)).iv2, triangles.at(newITriangles.at(it)).iv3) and
                e.iv2 == std::max(triangles.at(newITriangles.at(it)).iv2, triangles.at(newITriangles.at(it)).iv3))
            {
                triangles.at(newITriangles.at(it)).ie1 = ie;
            }

            // Triangle "it" (ie2)
            if (e.iv1 == std::min(triangles.at(newITriangles.at(it)).iv1, triangles.at(newITriangles.at(it)).iv3) and
                e.iv2 == std::max(triangles.at(newITriangles.at(it)).iv1, triangles.at(newITriangles.at(it)).iv3))
            {
                triangles.at(newITriangles.at(it)).ie2 = ie;
            }
        }
    }
}
