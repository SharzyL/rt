#include <cassert>
#include <stdexcept>
#include <vector>

#include <tiny_obj_loader.h>

#include "./obj_import.h"
#include "debug.h"
#include "objects/mesh.h"

namespace RT {

ObjImport::ObjImport(const std::string &obj_file_path) {
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(obj_file_path)) {
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

} // namespace RT
