#ifndef GROUP_OBJ_IMPORT
#define GROUP_OBJ_IMPORT

#include <vector>

#include "core/hit.h"
#include "core/ray.h"

#include "objects/object3d.h"
#include "objects/group.h"

namespace RT {

class ObjImport : public Group {

public:
    explicit ObjImport(const std::string &obj_file_path);

private:
    std::vector<Vector3f> all_vertices;
    std::vector<Material> all_materials;
};

} // namespace RT

#endif
