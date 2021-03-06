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
#include "objects/object3d.h"
#include "objects/rotate_bezier.h"

#include "core/ray.h"
#include "core/texture.h"
#include "core/material.h"
#include "core/light.h"

#include "./scene_parser.h"

namespace RT {

std::unique_ptr<Camera> SceneParser::parse_camera(const YAML::Node &node) {
    Vector3f pos = parse_vector3f(node["pos"].as<std::string>());
    Vector3f dir = parse_vector3f(node["dir"].as<std::string>());
    Vector3f up = parse_vector3f(node["up"].as<std::string>());
    auto width = node["width"].as<float>();
    auto height = node["height"].as<float>();
    auto angle = to_radian(node["angle"].as<float>());
    auto aperture = node["aperture"] ? node["aperture"].as<float>() : 0.f;
    auto focal_len = node["focal"] ? node["focal"].as<float>() : 1.f;
    auto shutter_time = node["shutter_time"] ? node["shutter_time"].as<float>() : 0.f;
    return std::make_unique<PerspectiveCamera>(pos, dir, up, width, height, angle, focal_len, aperture, shutter_time);
}

void SceneParser::parse_light(const YAML::Node &node) {
    const std::string &node_type = node["type"].as<std::string>();
    if (node_type == "point") {
        Vector3f center = parse_vector3f(node["center"].as<std::string>());
        Vector3f color = parse_vector3f(node["color"].as<std::string>());
        lights.emplace_back(std::make_unique<PointLight>(center, color));
    } else if (node_type == "sphere") {
        Vector3f center = parse_vector3f(node["center"].as<std::string>());
        auto radius = node["radius"].as<float>();
        Vector3f color = parse_vector3f(node["color"].as<std::string>());
        lights.emplace_back(std::make_unique<SphereLight>(center, radius, color));
    } else {
        CHECK(false) << "unsupported object type";
    }
}

Material *SceneParser::parse_material(const YAML::Node &node) {
    auto material = new Material((Material::IlluminationModel) node["illum"].as<int>());
    if (node["Ka"]) material->ambientColor = parse_vector3f(node["Ka"].as<std::string>());
    if (node["Kd"]) material->diffuseColor = parse_vector3f(node["Kd"].as<std::string>());
    if (node["Ks"]) material->specularColor = parse_vector3f(node["Ks"].as<std::string>());
    if (node["Ke"]) material->emissionColor = parse_vector3f(node["Ke"].as<std::string>());
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
    const std::string &node_type = node["type"].as<std::string>();

    if (node_type == "group") {
        auto group = new Group();
        for (const auto &sub_node: node["objects"]) {
            group->objects.emplace_back(parse_obj(sub_node));
        }
        return std::unique_ptr<Object3D>(group);

    } else if (node_type == "sphere") {
        auto center = parse_vector3f(node["center"].as<std::string>());
        auto radius = node["r"].as<float>();
        auto material = parse_material(node["mat"]);
        auto texture = parse_texture(node["texture"]);
        auto v = node["v"] ? parse_vector3f(node["v"].as<std::string>()) : Vector3f();
        return std::make_unique<Sphere>(center, radius, material, texture, v);

    } else if (node_type == "plane") {
        auto normal = parse_vector3f(node["normal"].as<std::string>());
        Vector3f texture_up = node["texture_up"]
                ? parse_vector3f(node["texture_up"].as<std::string>())
                : Vector3f(0, 1, 0);
        Vector2f texture_translate = node["texture_translate"]
              ? parse_vector2f(node["texture_translate"].as<std::string>())
              : Vector2f(0, 0);
        float texture_scale = node["texture_scale"]
                ? node["texture_scale"].as<float>()
                : 1.f;
        auto d = node["d"].as<float>();
        auto material = parse_material(node["mat"]);
        auto texture = parse_texture(node["texture"]);
        auto normal_texture = parse_texture(node["normal_texture"]);
        return std::make_unique<Plane>(normal, d, material, texture, normal_texture,
                                       texture_scale, texture_translate, texture_up);

    } else if (node_type == "triangle") {
        auto a = parse_vector3f(node["a"].as<std::string>());
        auto b = parse_vector3f(node["b"].as<std::string>());
        auto c = parse_vector3f(node["c"].as<std::string>());
        auto material = parse_material(node["mat"]);
        return std::make_unique<Triangle>(a, b, c, material);

    } else if (node_type == "load_obj") {
        const std::string &obj_file = node["obj"].as<std::string>();
        Vector3f scale = node["scale"] ? parse_vector3f(node["scale"].as<std::string>()) : Vector3f(1, 1, 1);
        Vector3f translate = node["translate"] ? parse_vector3f(node["translate"].as<std::string>()) : Vector3f(0, 0, 0);
        Material *material = node["mat"] ? parse_material(node["mat"]) : nullptr;
        Texture *texture = node["texture"] ? parse_texture(node["texture"]) : nullptr;
        return std::make_unique<ObjImport>(obj_file, scale, translate, material, texture);

    } else if (node_type == "rotate_bezier_mesh") {
        auto material = parse_material(node["mat"]);
        Texture *texture = node["texture"] ? parse_texture(node["texture"]) : nullptr;
        std::vector<Vector2f> controls;
        for (const auto &point_node: node["controls"]) {
            controls.emplace_back(parse_vector2f(point_node.as<std::string>()));
        }
        auto axis = parse_vector2f(node["axis"].as<std::string>());
        RotateBezier bezier(std::move(controls), axis, material, nullptr);
        auto density_x = node["density_x"].as<int>();
        auto density_y = node["density_y"].as<int>();

        return bezier.MakeMesh(material, texture, density_x, density_y);

    } else if (node_type == "rotate_bezier") {
        auto material = parse_material(node["mat"]);
        std::vector<Vector2f> controls;
        for (const auto &point_node: node["controls"]) {
            controls.emplace_back(parse_vector2f(point_node.as<std::string>()));
        }
        auto axis = parse_vector2f(node["axis"].as<std::string>());
        return std::make_unique<RotateBezier>(std::move(controls), axis, material, nullptr);

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

    YAML::Node lights_node = root_node["lights"];
    if (lights_node) {
        for (const auto &light_node: lights_node) {
            parse_light(light_node);
        }
    }

    YAML::Node bg_color_node = root_node["bg_color"];
    if (bg_color_node) {
        bg_color = parse_vector3f(bg_color_node.as<std::string>());
    }
}

} // namespace RT
