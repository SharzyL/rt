#ifndef GROUP_H
#define GROUP_H

#include <vector>

#include "core/hit.h"
#include "core/ray.h"

#include "objects/object3d.h"

class Group : public Object3D {

public:
    Group();
    ~Group();

    Group(const std::string &obj_file_path, const std::string &material_search_path);

    explicit Group(int num_objects);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    std::vector<Object3D *> objects;

private:
    std::vector<Vector3f> all_vertices;
    std::vector<Material> all_materials;
};

#endif
