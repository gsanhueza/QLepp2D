# QLepp2D

Author: Gabriel Sanhueza

QLepp2D is a Qt-based triangle mesh refiner and visualization software that uses the *Lepp-Delaunay* algorithm to improve a triangle mesh.

This application has been created to take advantage of GPUs, detecting and fixing bad
triangles in parallel.

Still, there's a CPU implementation available that can be used to compare the *speedup*
of GPU over CPU.

The GPU implementation has been made using OpenCL for maximum portability.

## TODO

* Implement the mesh improver in OpenCL

## DONE

* Implement the bad triangle detector in CPU.
* Implement the bad triangle detector in OpenCL.
