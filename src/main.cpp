#include <iostream>

#include "camera.hpp"
#include "group.hpp"
#include "image.hpp"
#include "light.hpp"
#include "scene_parser.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file> ss" << endl;
        return 1;
    }
    const char *inputFile = argv[1];
    const char *outputFile = argv[2]; // only bmp is allowed.

    SceneParser parser(inputFile);
    Camera *camera = parser.getCamera();
    Image img(camera->getWidth(), camera->getHeight());
    int w = camera->getWidth(), h = camera->getHeight();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            LOG(INFO) << fmt::format("cast ray ({}, {})", x, y);
            Ray r = camera->generateRay(Vector2f((float) x, (float) y));
            Group *baseGroup = parser.getGroup();
            Hit hit;
            bool isIntersect = baseGroup->intersect(r, hit, 0);
            if (isIntersect) {
                Vector3f finalColor = Vector3f::ZERO;
                for (int li = 0; li < parser.getNumLights(); li++) {
                    Light *light = parser.getLight(li);
                    Vector3f l, lightColor;
                    light->getIllumination(r.pointAtParameter(hit.getT()), l, lightColor);
                    finalColor += hit.getMaterial()->Shade(r, hit, l, lightColor);
                }
                img.SetPixel(x, y, finalColor);
                LOG(INFO) << fmt::format("paint ({}, {}) as {}", x, y, finalColor);
            } else {
                img.SetPixel(x, y, parser.getBackgroundColor());
            }
        }
    }
    img.SaveBMP(outputFile);
    return 0;
}
