//
// Created by jimmy on 8/25/25.
//

#include "Plane.hpp"

namespace Objects {

    Plane::Plane()
        : Renderable(RGB(0, 0, 255), 0, 0, glm::normalize(glm::vec3(0, 1, 0))), point(glm::vec3(0, 0, 1)), normal(axis) {}

    Plane::Plane(const RGB& color_, const int specular_, const float reflectivity_, const glm::vec3& axis_, const glm::vec3& point_)
        : Renderable(color_, specular_, reflectivity_, axis_), point(point_), normal(glm::normalize(axis_)) {}

    glm::vec3 Plane::get_point() const {
        return point;
    }
    glm::vec3 Plane::get_normal() const {
        return normal;
    }

    // Compute the intersections between the object and a given ray
    std::vector<float> Plane::intersect(const Ray& ray) const {
        const float denom {glm::dot(ray.get_direction(), normal)};

        if (std::abs(denom) < 1e-6) return {};

        const float t {glm::dot(point - ray.get_direction(), normal) / denom};

        if (t < 0) return {};

        return std::vector<float>{t};
    }

    // Compute surface normal at point P
    glm::vec3 Plane::normal_at(const glm::vec3& P) const {
        return normal;
    }

} // Objects