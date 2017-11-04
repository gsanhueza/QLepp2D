#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "CL/cl.h"
#include "vertex.h"

typedef struct {
    cl_int i1;
    cl_int i2;
    cl_int i3;
    cl_int bad;
} Triangle;

#endif // TRIANGLE_H
