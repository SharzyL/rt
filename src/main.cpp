#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "camera.h"
#include "debug.h"
#include "image.h"
#include "light.h"

using namespace std;

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "testcases";
    tinyobj::ObjReader reader;
    reader.ParseFromFile("testcases/CornellBox-Original.obj");
    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    Image img(1024, 768);
    img.SaveBMP("/tmp/output.bmp");
    return 0;
}
