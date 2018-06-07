#include <model.h>

int main()
{
    Model model;
    model.setOpenCLEngine();
    model.loadFile("A.off");
    model.detectBadTriangles(50.0);
    model.improveTriangulation();
    model.saveFile("B.off");
    return 0;
}
