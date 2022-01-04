#ifndef OBJECT3D_H
#define OBJECT3D_H

namespace RT {

class Hit;
class Material;
class Texture;
class Ray;

// Base class for all 3d entities.
class Object3D {
public:
    virtual ~Object3D() = default;

    // MayIntersect Ray with this object. If hit, store information in hit
    // structure.
    virtual bool Intersect(const Ray &r, Hit &h, float tmin) const = 0;
};

class SimpleObject3D : public Object3D {
public:
    SimpleObject3D(const Material *material, const Texture *texture) : material(material), texture(texture){};

    [[nodiscard]] virtual Vector3f AmbientColorAtHit(const Hit &hit) const = 0;

    [[nodiscard]] const Material *GetMaterial() const { return material; }

    const Material *material;
    const Texture *texture; // maybe nullptr, which means no texture for it
};

} // namespace RT

#endif
