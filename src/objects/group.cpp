#include <stdexcept>
#include <vector>
#include <cassert>

#include <tiny_obj_loader.h>

#include "./group.h"
#include "objects/mesh.h"
#include "debug.h"

Group::Group() = default;

Group::Group(int num_objects) { objects.reserve(num_objects); }

bool Group::intersect(const Ray &r, Hit &h, float tmin) const {
    bool is_intersect = false;
    for (const auto obj : objects) {
        if (obj->intersect(r, h, tmin)) {
            is_intersect = true;
        }
    }
    return is_intersect;
}

size_t Group::getGroupSize() const { return objects.size(); }

Group::Group(const std::string &obj_file_path, const std::string &material_search_path) {

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
    vs.reserve(v_num_3 / 3);
    for (size_t i = 0; i < v_num_3; i += 3) {
        vs.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    // prepare materials
    size_t mat_num = materials.size();
    mats.reserve(mat_num);
    for (const auto &mat: materials) {
        mats.emplace_back(mat);
    }

    for (const auto & shape : shapes) {  // iterate shapes
        LOG(INFO) << fmt::format("shape '{}': {} faces", shape.name, shape.mesh.num_face_vertices.size());
        Mesh *mesh = new Mesh(vs, mats, shape.mesh.num_face_vertices.size());

        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {   // iterate faces
            const tinyobj::material_t &material = materials[shape.mesh.material_ids[f]];
            LOG(INFO) << fmt::format("|-face of {} vertices", shape.mesh.num_face_vertices[f]);
            LOG(INFO) << fmt::format("|-material {}", material.name);
            assert(shape.mesh.num_face_vertices[f] == 3);
            mesh->t.emplace_back(
                shape.mesh.indices[index_offset + 0].vertex_index,
                shape.mesh.indices[index_offset + 1].vertex_index,
                shape.mesh.indices[index_offset + 2].vertex_index
            );
            mesh->mat_idx.emplace_back();
            index_offset += 3;
        }
        mesh->computeNormal();
        objects.emplace_back(mesh);
    }
}

Group::~Group() {
    for (Object3D *ptr: objects) {
        delete ptr;
    }
}
