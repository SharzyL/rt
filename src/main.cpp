#include "core/camera.h"
#include "core/renderer.h"
#include "debug.h"
#include "image.h"
#include "util.h"

#include "objects/group.h"

using namespace std;

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    PerspectiveCamera camera =
        PerspectiveCamera(Vector3f(0, 1.3, 3), Vector3f(0, -0.13, -1), Vector3f(0, 1, 0), 300, 300, to_radian(50.f));
    Group g("testcases/CornellBox-Original.obj", "testcases");

    Renderer renderer(3, 2);
    renderer.Render(g, camera, "output/test.bmp");
    return 0;
}
