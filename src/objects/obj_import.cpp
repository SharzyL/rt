#include <cassert>
#include <stdexcept>
#include <vector>

#include <tiny_obj_loader.h>

#include "./obj_import.h"
#include "debug.h"
#include "objects/mesh.h"

namespace RT {

ObjImport::ObjImport() = default;

ObjImport::ObjImport(int num_objects) { objects.reserve(num_objects); }

bool ObjImport::Intersect(const Ray &r, Hit &h, float tmin) const {
    bool is_intersect = false;
    for (const auto obj : objects) {
        if (obj->Intersect(r, h, tmin)) {
            is_intersect = true;
        }
    }
    return is_intersect;
}

ObjImport::ObjImport(const std::string &obj_file_path, const std::string &material_search_path) {

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = material_search_path;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(obj_file_path, reader_config)) {
        if (!reader.Error().empty()) {
            throw std::runtime_error(fmt::format("TinyObjReader: {}", reader.Error()));
        }
    }
    if (!reader.Warning().empty()) {
        LOG(WARNING) << fmt::format("TinyObjReader: {}", reader.Warning());
    }

    const tinyobj::attrib_t &attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t> &shapes = reader.GetShapes();
    const std::vector<tinyobj::material_t> &materials = reader.GetMaterials();

    // prepare vertices
    size_t v_num_3 = attrib.vertices.size();
    all_vertices.reserve(v_num_3 / 3);
    for (size_t i = 0; i < v_num_3; i += 3) {
        all_vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    // prepare materials
    size_t mat_num = materials.size();
    assert(mat_num > 0);
    all_materials.reserve(mat_num);
    for (const auto &mat : materials) {
        all_materials.emplace_back(mat);
    }

    for (const auto &shape : shapes) { // iterate shapes
        objects.emplace_back(new Mesh(all_vertices, all_materials, shape));
    }
}

ObjImport::~ObjImport() {
    for (Object3D *ptr : objects) {
        delete ptr;
    }
}

} // namespace RT
