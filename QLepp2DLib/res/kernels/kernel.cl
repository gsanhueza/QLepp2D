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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#if __OPENCL_VERSION__ <= CL_VERSION_1_1
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

typedef struct {
    float x;
    float y;
    float z;
} Vertex;

typedef struct {
    int iv1;
    int iv2;
    int iv3;
    int ie1;
    int ie2;
    int ie3;
    int bad;
    int valid;
} Triangle;

typedef struct {
    int ita;
    int itb;
    int isTerminalEdge;
} Edge;

#if 0
int get_global_id(int);
float pown(float, int);
float sqrt(float);
float acos(float);
#endif

/* Each thread is a Triangle */
kernel void detectBadTriangles(const double angle, global Triangle *triangles, global Vertex *vertices)
{
    int idx = get_global_id(0);

    Vertex A, B, C;
    A = vertices[triangles[idx].iv1];
    B = vertices[triangles[idx].iv2];
    C = vertices[triangles[idx].iv3];

    float length_a2 = pown(B.x - C.x, 2) + pown(B.y - C.y, 2) + pown(B.z - C.z, 2);
    float length_b2 = pown(A.x - C.x, 2) + pown(A.y - C.y, 2) + pown(A.z - C.z, 2);
    float length_c2 = pown(A.x - B.x, 2) + pown(A.y - B.y, 2) + pown(A.z - B.z, 2);


    float length_a = sqrt(length_a2);
    float length_b = sqrt(length_b2);
    float length_c = sqrt(length_c2);

    float angle_opp_a2 = acos((length_b2 + length_c2 - length_a2) / (2 * length_b * length_c));
    float angle_opp_b2 = acos((length_a2 + length_c2 - length_b2) / (2 * length_a * length_c));
    float angle_opp_c2 = acos((length_a2 + length_b2 - length_c2) / (2 * length_a * length_b));

    float rad_angle = angle * M_PI / 180.0;

    triangles[idx].bad = (angle_opp_a2 < rad_angle || angle_opp_b2 < rad_angle || angle_opp_c2 < rad_angle);
}

/* Each thread is a Triangle */
kernel void improveTriangulation(global Triangle *triangles, global Vertex *vertices, global int *indices)
{
    // TODO Kernel implementation
    int idx = get_global_id(0);

    triangles[idx].bad = 0;
}

/* Each thread is a Triangle */
kernel void detectTerminalEdges(global Edge *edges,
                                global Vertex *vertices,
                                global Triangle *triangles,
                                global int *triangleHistory)
{
    int idx = get_global_id(0);

    /* triangleHistory is a vector of size 3 brought from the host.
     * We'll use it as a circular vector, so we can just check
     * if triangleHistory[k] == triangleHistory[(k + 2) % 3].
     */
    Triangle t = triangles[idx];
    int it = idx;
    int k = 0; // Index of triangleHistory

    if (t.bad)
    {
        while (true)
        {
            // Add myself to the history.
            triangleHistory[k] = it;

            // Detect longest edge.
            Vertex A, B, C;
            A = vertices[t.iv1];
            B = vertices[t.iv2];
            C = vertices[t.iv3];

            float length_a2 = pown(B.x - C.x, 2) + pown(B.y - C.y, 2) + pown(B.z - C.z, 2);
            float length_b2 = pown(A.x - C.x, 2) + pown(A.y - C.y, 2) + pown(A.z - C.z, 2);
            float length_c2 = pown(A.x - B.x, 2) + pown(A.y - B.y, 2) + pown(A.z - B.z, 2);

            int neighbourIT;
            int longestIE;
            Edge longestEdge;

            if (length_a2 >= length_b2 && length_a2 >= length_c2)
            {
                longestEdge = edges[t.ie1];
                longestIE = t.ie1;
            }
            else if (length_b2 >= length_a2 && length_b2 >= length_c2)
            {
                longestEdge = edges[t.ie2];
                longestIE = t.ie2;
            }
            else
            {
                longestEdge = edges[t.ie3];
                longestIE = t.ie3;
            }

            // Detect my neighbour.
            neighbourIT = (longestEdge.ita == it) ? longestEdge.itb : longestEdge.ita;

            // Border triangle
            if (neighbourIT < 0)
            {
                edges[longestIE].isTerminalEdge = 1;
                break;
            }

            // If I was here before, then I found the final edge of Lepp.
            if (it == triangleHistory[(k + 1) % 3])     // Equivalent of (k - 2)
            {
                edges[longestIE].isTerminalEdge = 1;
                break;
            }

            // Update t to check neighbour
            it = neighbourIT;
            t = triangles[neighbourIT];
            k = (k + 1) % 3;
        }
    }
}
