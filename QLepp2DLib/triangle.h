#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"

typedef struct {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    int bad;
} Triangle;

#endif // TRIANGLE_H
