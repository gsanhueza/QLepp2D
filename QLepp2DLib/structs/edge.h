#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

// FIXME This structure can have the triangle containing the edge
typedef struct {
    Vertex a;
    Vertex b;
} Edge;
#endif // EDGE_H
