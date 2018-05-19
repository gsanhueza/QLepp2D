#include <model.h>

int main()
{
    Model model;
    model.setOpenCLEngine();
    model.loadOFF("A.off");
    model.detectBadTriangles(50.0);
    model.improveTriangulation();
    model.saveOFF("B.off");
    return 0;
}
