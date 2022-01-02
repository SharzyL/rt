#include <cassert>

#include <yaml-cpp/yaml.h>

#include "objects/sphere.h"
#include "objects/plane.h"
#include "objects/triangle.h"
#include "objects/group.h"
#include "objects/obj_import.h"
#include "objects/object3d.h"

#include "scene_parser.h"

namespace RT {
//static std::unique_ptr<Camera> parse_camera(const YAML::Node &node) {
//
//}
//
//static std::unique_ptr<Object3D> parse_material(const YAML::Node &node) {
//
//}
//
//static std::unique_ptr<Object3D> parse_group(const YAML::Node &node) {
//
//}
//
//static std::unique_ptr<Object3D> parse_sphere(const YAML::Node &node) {
//
//}
//
//static std::unique_ptr<Object3D> parse_plane(const YAML::Node &node) {
//
//}
//
//static std::unique_ptr<Object3D> parse_obj_import(const YAML::Node &node) {
//
//}

void SceneParser::parse(const std::string &scene_file) {
//    YAML::Node scene_node = YAML::LoadFile(scene_file);
//    assert(scene_node.IsMap());
//    YAML::Node camera_node = scene_node["camera"];
//    camera = parse_camera(camera_node);
//    YAML::Node world_node = scene_node["world"];
//    Object3D *se = new Group;
//    scene = std::make_unique(se);
}

} // namespace RT
