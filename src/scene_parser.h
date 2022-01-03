#ifndef RT_SCENEPARSER_H
#define RT_SCENEPARSER_H

#include <string>
#include <memory>
#include <map>

#include <yaml-cpp/yaml.h>

#include "objects/object3d.h"
#include "core/camera.h"
#include "core/texture.h"

namespace RT {

class SceneParser {
public:
    SceneParser() = default;

    void parse(const std::string &scene_file);

    std::unique_ptr<Camera> camera;  // use pointer because it is an abstract class
    std::unique_ptr<Object3D> scene; // same as above
private:
    std::unique_ptr<Object3D> parse_obj(const YAML::Node &node);

    std::unique_ptr<Camera> parse_camera(const YAML::Node &node);
    Material *parse_material(const YAML::Node &node);  // no ownership transfer, no need of unique_ptr
    Texture *parse_texture(const YAML::Node &node);

    std::vector<std::unique_ptr<Material>> all_materials;
    std::vector<std::unique_ptr<Texture>> all_textures;
};

} // namespace RT

#endif //RT_SCENEPARSER_H
