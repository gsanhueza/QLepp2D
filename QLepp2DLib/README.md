# Example

```
// Compile with "g++ file.cpp -o file -I/usr/include/QLepp2D -lOpenCL -lqlepp2d-lib"

#include <model.h>

int main()
{
    Model model;
    model.setOpenCLEngine();
    model.loadOFF("/home/user/A.off");
    model.detectBadTriangles(25.0);
    model.improveTriangulation();
    model.saveOFF("/home/user/B.off");
    return 0;
}

```
