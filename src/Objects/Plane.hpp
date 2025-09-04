//
// Created by jimmy on 8/25/25.
//

#ifndef RAYTRACINGCPP_PLANE_H
#define RAYTRACINGCPP_PLANE_H
#include "Renderable.hpp"

namespace Objects {
    class Plane : public Renderable {
        glm::vec3 point;
        glm::vec3 normal;

    public:
        Plane();
        Plane(const RGB& color_, int specular_, float reflectivity_, const glm::vec3& axis_, const glm::vec3& point_);

        glm::vec3 get_point() const;
        glm::vec3 get_normal() const;

        // Compute the intersections between the object and a given ray
        std::vector<float> intersect(const Ray& ray) const override;

        // Compute surface normal at point P
        glm::vec3 normal_at(const glm::vec3& P) const override;
    };
} // Objects

#endif //RAYTRACINGCPP_PLANE_H