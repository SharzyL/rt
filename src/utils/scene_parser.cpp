#include <cassert>
#include <memory>

#include "yaml-cpp/yaml.h"

#include "Vector3f.h"

#include "math_util.h"
#include "debug.h"

#include "objects/sphere.h"
#include "objects/plane.h"
#include "objects/triangle.h"
#include "objects/group.h"
#include "objects/obj_import.h"
#include "objects/mesh.h"
#include "objects/object3d.h"

#include "core/ray.h"
#include "core/texture.h"
#include "core/material.h"

#include "./scene_parser.h"

namespace RT {

std::unique_ptr<Camera> SceneParser::parse_camera(const YAML::Node &node) {
    Vector3f pos = parse_vector(node["pos"].as<std::string>());
    Vector3f dir = parse_vector(node["dir"].as<std::string>());
    Vector3f up = parse_vector(node["up"].as<std::string>());
    auto width = node["width"].as<float>();
    auto height = node["height"].as<float>();
    auto angle = to_radian(node["angle"].as<float>());
    return std::make_unique<PerspectiveCamera>(pos, dir, up, width, height, angle);
}

Material *SceneParser::parse_material(const YAML::Node &node) {
    auto material = new Material((Material::IlluminationModel) node["illum"].as<int>());
    if (node["Ka"]) material->ambientColor = parse_vector(node["Ka"].as<std::string>());
    if (node["Kd"]) material->diffuseColor = parse_vector(node["Kd"].as<std::string>());
    if (node["Ks"]) material->specularColor = parse_vector(node["Ks"].as<std::string>());
    if (node["Ke"]) material->emissionColor = parse_vector(node["Ke"].as<std::string>());
    if (node["Ns"]) material->shininess = node["Ns"].as<float>();
    if (node["Ni"]) material->refraction = node["Ni"].as<float>();
    if (node["name"]) material->name = node["name"].as<std::string>();
    all_materials.emplace_back(material);  // transfer ownership to all_materials
    return material;
}

Texture *SceneParser::parse_texture(const YAML::Node &node) {
    if (node) {
        return all_textures.emplace_back(std::make_unique<MappedTexture>(node["file"].as<std::string>(), gamma)).get();
    } else {
        return nullptr;
    }
}

std::unique_ptr<Object3D> SceneParser::parse_obj(const YAML::Node &node) {
    CHECK(node.IsMap());
    const std::string &node_type = node["type"].as<std::string>();

    if (node_type == "group") {
        auto group = new Group();
        for (const auto &sub_node: node["objects"]) {
            group->objects.emplace_back(parse_obj(sub_node));
        }
        return std::unique_ptr<Object3D>(group);

    } else if (node_type == "sphere") {
        auto center = parse_vector(node["center"].as<std::string>());
        auto radius = node["r"].as<float>();
        auto material = parse_material(node["mat"]);
        auto texture = parse_texture(node["texture"]);
        return std::make_unique<Sphere>(center, radius, material, texture);

    } else if (node_type == "plane") {
        auto normal = parse_vector(node["normal"].as<std::string>());
        auto d = node["d"].as<float>();
        auto material = parse_material(node["mat"]);
        auto texture = parse_texture(node["texture"]);
        return std::make_unique<Plane>(normal, d, material, texture);

    } else if (node_type == "triangle") {
        auto a = parse_vector(node["a"].as<std::string>());
        auto b = parse_vector(node["b"].as<std::string>());
        auto c = parse_vector(node["c"].as<std::string>());
        auto material = parse_material(node["mat"]);
        return std::make_unique<Triangle>(a, b, c, material);

    } else if (node_type == "load_obj") {
        const std::string &obj_file = node["obj"].as<std::string>();
        Vector3f scale = node["scale"] ? parse_vector(node["scale"].as<std::string>()) : Vector3f(1, 1, 1);
        Vector3f translate = node["translate"] ? parse_vector(node["translate"].as<std::string>()) : Vector3f(0, 0, 0);
        Material *material = node["mat"] ? parse_material(node["mat"]) : nullptr;
        return std::make_unique<ObjImport>(obj_file, scale, translate, material);

    } else if (node_type == "bbox") {
        auto material = parse_material(node["mat"]);
        auto bbox = new BoundingBox(material);
        for (const auto &point_node: node["points"]) {
            bbox->AddVertex(parse_vector(point_node.as<std::string>()));
        }
        return std::unique_ptr<Object3D>(bbox);

    } else {
        CHECK(false) << "unsupported object type";
    }
}

void SceneParser::parse(const std::string &scene_file) {
    YAML::Node root_node = YAML::LoadFile(scene_file);

    YAML::Node camera_node = root_node["camera"];
    camera = parse_camera(camera_node);

    YAML::Node gamma_node = root_node["gamma"];
    if (gamma_node) gamma = gamma_node.as<float>();

    YAML::Node world_node = root_node["world"];
    auto world_group = new Group;
    for (const auto &obj_node: world_node) {
        world_group->objects.emplace_back(parse_obj(obj_node));
    }
    scene.reset(world_group);
}


} // namespace RT
