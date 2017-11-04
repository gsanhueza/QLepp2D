#include <QDebug>
#include <cmath>
#include "cpuengine.h"
#include "triangle.h"
#include "edge.h"

CPUEngine::CPUEngine()
{
}

bool CPUEngine::detectBadTriangles( double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices)
{
    // TODO
    // FIXME Maybe indices vector is not needed in this method.
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

        qDebug() << "Edge A = (" << A.a.x << ", " << A.a.y << ") <--> " << "(" << A.b.x << ", " << A.b.y << ")";
        qDebug() << "Edge B = (" << B.a.x << ", " << B.a.y << ") <--> " << "(" << B.b.x << ", " << B.b.y << ")";
        qDebug() << "Edge C = (" << C.a.x << ", " << C.a.y << ") <--> " << "(" << C.b.x << ", " << C.b.y << ")";

        float length_a2 = pow(A.a.x - A.b.x, 2) + pow(A.a.y - A.b.y, 2) + pow(A.a.z - A.b.z, 2);
        float length_b2 = pow(B.a.x - B.b.x, 2) + pow(B.a.y - B.b.y, 2) + pow(B.a.z - B.b.z, 2);
        float length_c2 = pow(C.a.x - C.b.x, 2) + pow(C.a.y - C.b.y, 2) + pow(C.a.z - C.b.z, 2);

        qDebug() << "length_a2 =" << length_a2;
        qDebug() << "length_b2 =" << length_b2;
        qDebug() << "length_c2 =" << length_c2;

        float length_a = sqrt(length_a2);
        float length_b = sqrt(length_b2);
        float length_c = sqrt(length_c2);

        qDebug() << "length_a =" << length_a;
        qDebug() << "length_b =" << length_b;
        qDebug() << "length_c =" << length_c;

        float angle_opp_a2 = std::acos((length_b2 + length_c2 - length_a2) / (2 * length_b * length_c));
        float angle_opp_b2 = std::acos((length_a2 + length_c2 - length_b2) / (2 * length_a * length_c));
        float angle_opp_c2 = std::acos((length_a2 + length_b2 - length_c2) / (2 * length_a * length_b));

        float rad_angle = angle * M_PI / 180.0;

        qDebug() << "angle_opp_a2:" << angle_opp_a2;
        qDebug() << "angle_opp_b2:" << angle_opp_b2;
        qDebug() << "angle_opp_c2:" << angle_opp_c2;
        qDebug() << "rad_angle:" << rad_angle;
        qDebug() << "Is bad: " << (angle_opp_a2 < rad_angle or angle_opp_b2 < rad_angle or angle_opp_c2 < rad_angle);
        qDebug() << "-------------------";

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
