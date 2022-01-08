#include <stdexcept>
#include <vector>

#include <tiny_obj_loader.h>

#include "utils/debug.h"
#include "core/material.h"
#include "objects/mesh.h"

#include "./obj_import.h"

namespace RT {

ObjImport::ObjImport(const std::string &obj_file_path,
                     const Vector3f &scale,
                     const Vector3f &translate,
                     const Material *default_mat,
                     const Texture *default_tex) {
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
        auto v = Vector3f(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
        all_vertices.emplace_back(v * scale + translate);
    }

    // prepare normals
    size_t n_num_3 = attrib.normals.size();
    all_normals.reserve(n_num_3 / 3);
    for (size_t i = 0; i < n_num_3; i += 3) {
        auto v = Vector3f(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]);
        all_normals.emplace_back(v * scale + translate);
    }

    // prepare texture coordinates
    size_t t_num_2 = attrib.texcoords.size();
    all_vertices.reserve(t_num_2 / 2);
    for (size_t i = 0; i < t_num_2; i += 2) {
        auto v = Vector2f(attrib.texcoords[i], attrib.texcoords[i + 1]);
        all_tex_cord.emplace_back(v);
    }

    // prepare materials
    size_t mat_num = materials.size();

    all_materials.reserve(mat_num);
    for (const auto &mat: materials) {
        all_materials.emplace_back(mat);
    }

    for (const auto &shape: shapes) { // iterate shapes
        objects.emplace_back(new Mesh(
                all_vertices,
                all_normals,
                all_tex_cord,
                all_materials,
                shape,
                default_mat,
        default_tex
        ));
    }
}

} // namespace RT
