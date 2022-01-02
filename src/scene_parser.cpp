#include <cassert>
#include <memory>

#include <Vector3f.h>

#include "util.h"

#include "objects/sphere.h"
#include "objects/plane.h"
#include "objects/triangle.h"
#include "objects/group.h"
#include "objects/obj_import.h"
#include "objects/object3d.h"

#include "scene_parser.h"

namespace RT {

static std::unique_ptr<Camera> parse_camera(const YAML::Node &node) {
    Vector3f pos = parse_vector(node["pos"].as<std::string>());
    Vector3f dir = parse_vector(node["dir"].as<std::string>());
    Vector3f up = parse_vector(node["up"].as<std::string>());
    auto width = node["width"].as<float>();
    auto height = node["height"].as<float>();
    auto angle = to_radian(node["angle"].as<float>());
    return std::make_unique<PerspectiveCamera>(pos, dir, up, width, height, angle);
}

static std::unique_ptr<Material> parse_material(const YAML::Node &node) {
    auto material = std::make_unique<Material>((Material::IlluminationModel) node["illum"].as<int>());
    if (node["Ka"]) material->ambientColor = parse_vector(node["Ka"].as<std::string>());
    if (node["Kd"]) material->diffuseColor = parse_vector(node["Kd"].as<std::string>());
    if (node["Ks"]) material->specularColor = parse_vector(node["Ks"].as<std::string>());
    if (node["Ke"]) material->emissionColor = parse_vector(node["Ke"].as<std::string>());
    if (node["Ns"]) material->ambientColor = node["Ns"].as<float>();
    if (node["Ni"]) material->refraction = node["Ni"].as<float>();
    if (node["name"]) material->name = node["name"].as<std::string>();
    return material;
}

std::unique_ptr<Object3D> SceneParser::parse_obj(const YAML::Node &node) {
    assert(node.IsMap());
    const std::string &node_type = node["type"].as<std::string>();

    if (node_type == "group") {
        assert(node["objects"].IsSequence());
        auto group = new Group();
        for (const auto &sub_node: node["objects"]) {
            group->objects.emplace_back(parse_obj(sub_node));
        }
        return std::unique_ptr<Object3D>(group);

    } else if (node_type == "sphere") {
        Vector3f center = parse_vector(node["center"].as<std::string>());
        auto radius = node["r"].as<float>();
        all_materials.emplace_back(parse_material(node["mat"]));
        return std::make_unique<Sphere>(center, radius, all_materials.back().get());

    } else if (node_type == "plane") {
        Vector3f normal = parse_vector(node["normal"].as<std::string>());
        auto d = node["d"].as<float>();
        all_materials.emplace_back(parse_material(node["mat"]));
        return std::make_unique<Plane>(normal, d, all_materials.back().get());

    } else if (node_type == "triangle") {
        Vector3f a = parse_vector(node["a"].as<std::string>());
        Vector3f b = parse_vector(node["b"].as<std::string>());
        Vector3f c = parse_vector(node["c"].as<std::string>());
        all_materials.emplace_back(parse_material(node["mat"]));
        return std::make_unique<Triangle>(a, b, c, all_materials.back().get());

    } else if (node_type == "load_obj") {
        const std::string &obj_file = node["obj"].as<std::string>();
        return std::make_unique<ObjImport>(obj_file);

    } else {
        throw std::runtime_error("unsupported object type");
    }
}

void SceneParser::parse(const std::string &scene_file) {
    YAML::Node root_node = YAML::LoadFile(scene_file);
    assert(root_node.IsMap());

    YAML::Node camera_node = root_node["camera"];
    camera = parse_camera(camera_node);

    YAML::Node world_node = root_node["world"];
    assert(world_node.IsSequence());
    auto world_group = new Group;
    for (const auto &obj_node: world_node) {
        world_group->objects.emplace_back(parse_obj(obj_node));
    }
    scene.reset(world_group);
}

} // namespace RT
