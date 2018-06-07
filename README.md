# QLepp2D

Author: Gabriel Sanhueza

QLepp2D is a Qt-based triangle mesh refiner and visualization software that uses
the *Lepp-Centroid* algorithm to improve a triangle mesh.

This application has been created to take advantage of GPUs, detecting and
fixing bad triangles in parallel.

Still, there's a CPU implementation available that can be used to compare the
*speedup* of GPU over CPU.

The GPU implementation has been made using OpenCL for maximum portability.

## Usage

If you're reading this README.md, it means that you'll use the full GUI+LIB.

* Load a mesh file. (Only OFF files are currently supported at the moment.)
* Set a desired minimum angle.
* Check bad triangles (Detect button)
* Improve bad triangles.
* Save your new mesh.

Check the Help menu for more details.
