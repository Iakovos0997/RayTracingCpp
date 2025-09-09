//
// Created by jimmy on 8/25/25.
//

#ifndef RAYTRACINGCPP_CYLINDER_H
#define RAYTRACINGCPP_CYLINDER_H
#include "IRenderable.hpp"

namespace Objects {
    class Cylinder : public IRenderable {
        glm::vec3 base_center;
        float radius;
        float height;

    public:
        explicit Cylinder(const glm::vec3 &base_center_, float radius_, float height_);
        Cylinder(const glm::vec3 &base_center_, float radius_, float height_, const RGB& color_, int specular_, float reflectivity_, const glm::vec3& axis_);

        // Getters
        glm::vec3 get_base_center() const;
        float get_radius() const;
        float get_height() const;

        std::vector<float> intersect(const Ray& ray) const override;

        // Compute surface normal at point P
        glm::vec3 normal_at(const glm::vec3& P) const override;
    };
} // Objects

#endif //RAYTRACINGCPP_CYLINDER_H