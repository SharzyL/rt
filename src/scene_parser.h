#ifndef RT_SCENEPARSER_H
#define RT_SCENEPARSER_H

#include <string>
#include <memory>
#include <map>

#include <yaml-cpp/yaml.h>

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
    std::unique_ptr<Object3D> parse_obj(const YAML::Node &node);
    std::vector<std::unique_ptr<const Material>> all_materials;
};

} // namespace RT

#endif //RT_SCENEPARSER_H
