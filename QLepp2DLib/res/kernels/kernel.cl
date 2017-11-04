typedef struct {
    float x;
    float y;
    float z;
} Vertex;

typedef struct {
    Vertex a;
    Vertex b;
} Edge;

typedef struct {
    int i1;
    int i2;
    int i3;
    int bad;
} Triangle;

kernel void detectBadTriangles(global double *angleVect, global Triangle *triangles, global Vertex *vertices)
{
    int idx = get_global_id(0);
    double angle = angleVect[0];

    Edge A, B, C;
    A.a = vertices[triangles[idx].i1];
    A.b = vertices[triangles[idx].i2];
    B.a = vertices[triangles[idx].i2];
    B.b = vertices[triangles[idx].i3];
    C.a = vertices[triangles[idx].i3];
    C.b = vertices[triangles[idx].i1];

    float length_a2 = pown(A.a.x - A.b.x, 2) + pown(A.a.y - A.b.y, 2) + pown(A.a.z - A.b.z, 2);
    float length_b2 = pown(B.a.x - B.b.x, 2) + pown(B.a.y - B.b.y, 2) + pown(B.a.z - B.b.z, 2);
    float length_c2 = pown(C.a.x - C.b.x, 2) + pown(C.a.y - C.b.y, 2) + pown(C.a.z - C.b.z, 2);

    float length_a = sqrt(length_a2);
    float length_b = sqrt(length_b2);
    float length_c = sqrt(length_c2);

    float angle_opp_a2 = acos((length_b2 + length_c2 - length_a2) / (2 * length_b * length_c));
    float angle_opp_b2 = acos((length_a2 + length_c2 - length_b2) / (2 * length_a * length_c));
    float angle_opp_c2 = acos((length_a2 + length_b2 - length_c2) / (2 * length_a * length_b));

    float rad_angle = angle * M_PI / 180.0;

    triangles[idx].bad = (angle_opp_a2 < rad_angle || angle_opp_b2 < rad_angle || angle_opp_c2 < rad_angle);
}
