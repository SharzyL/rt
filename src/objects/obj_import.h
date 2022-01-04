#ifndef RT_OBJ_IMPORT
#define RT_OBJ_IMPORT

#include <vector>

#include "objects/group.h"

namespace RT {

class ObjImport : public Group {

public:
    explicit ObjImport(const std::string &obj_file_path, const Vector3f &scale = Vector3f(1, 1, 1),
                       const Vector3f &translate = Vector3f(1, 1, 1), const Material *default_mat = nullptr);

private:
    std::vector<Vector3f> all_vertices;
    std::vector<Material> all_materials;
};

} // namespace RT

#endif
