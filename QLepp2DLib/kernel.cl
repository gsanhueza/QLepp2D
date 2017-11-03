typedef struct {
    float x;
    float y;
    float z;
} Vertex;

typedef struct {
    int i1;
    int i2;
    int i3;
    int bad;
} Triangle;

kernel void detectBadTriangles(global Triangle *triangles, global Vertex *vertices)
{
    int idx = get_global_id(0);
}
