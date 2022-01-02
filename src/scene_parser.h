#ifndef RT_SCENEPARSER_H
#define RT_SCENEPARSER_H

#include <string>
#include <memory>
#include <map>

#include "objects/object3d.h"
#include "core/camera.h"

namespace RT {

class SceneParser {
public:
    SceneParser() = default;

    void parse(const std::string &scene_file);

    std::unique_ptr<Camera> camera;
    std::unique_ptr<Object3D> scene;

private:
    std::map<std::string, Material> materials;
};

} // namespace RT

#endif //RT_SCENEPARSER_H
