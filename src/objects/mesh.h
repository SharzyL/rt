#ifndef MESH_H
#define MESH_H

#include <vector>

#include <tiny_obj_loader.h>

#include "Vector2f.h"
#include "Vector3f.h"

#include "objects/object3d.h"
#include "objects/triangle.h"

class Mesh : public Object3D {

public:
    Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, size_t size);

    struct TriangleIndex {
        TriangleIndex(int idx1, int idx2, int idx3): x{idx1, idx2, idx3} {}
        int &operator[](int i) { return x[i]; }
        [[nodiscard]] int at(int i) const { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front
        // face
        int x[3]{};
    };

    std::vector<TriangleIndex> t;
    std::vector<int> mat_idx;
    std::vector<Vector3f> n;
    bool intersect(const Ray &r, Hit &h, float tmin) const override;

    // Normal can be used for light estimation
    void computeNormal();

private:
    const std::vector<Vector3f> &vs;
    const std::vector<Material> &mats;
};

#endif
