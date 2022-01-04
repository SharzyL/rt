#ifndef RT_OBJ_IMPORT
#define RT_OBJ_IMPORT

#include <vector>

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
