//
// Created by jimmy on 8/25/25.
//

#include "Cylinder.h"

namespace Objects {

    Cylinder::Cylinder(const glm::vec3 &base_center_, const float radius_, const float height_)
        : Renderable(), base_center(base_center_), radius(radius_), height(height_) {}

    Cylinder::Cylinder(const glm::vec3 &base_center_, const float radius_, const float height_, const RGB& color_, const int specular_, const float reflectivity_, const glm::vec3& axis_)
        : Renderable(color_, specular_, reflectivity_, axis_), base_center(base_center_), radius(radius_), height(height_) {}

    // Getters
    glm::vec3 Cylinder::get_base_center() const {
        return base_center;
    }
    float Cylinder::get_radius() const {
        return radius;
    }
    float Cylinder::get_height() const {
        return height;
    }

    std::vector<float> Cylinder::intersect(const Ray& ray) const {
        auto CO {ray.get_origin() - base_center};

        // Project D and CO onto plane perpendicular to axis
        auto D_proj {ray.get_direction() - axis *glm::dot(ray.get_direction(), axis)};
        auto CO_proj {CO - axis * glm::dot(CO, axis)};

        auto a {glm::dot(D_proj, D_proj)};
        auto b {2 * glm::dot(ray.get_direction(), CO_proj)};
        auto c {glm::dot(CO_proj, CO_proj) - std::pow(radius, 2)};

        std::vector<float> t_side;
        t_side.reserve(2);

        auto disc {b * b - 4 * a * c};
        if (disc > 0) {
            auto sqrt_disc {std::sqrt(disc)};
            for (const auto&  t : {((-b - sqrt_disc) / (2 * a)), ((-b + sqrt_disc) / (2 * a))}) {
                auto P {ray.at(t)};
                auto h {glm::dot(P - base_center, axis)};
                if (h >= 0 && h <= height) {
                    t_side.push_back(t);
                }
            }
        }

        // Check Caps
        std::vector<float> t_caps;
        t_caps.reserve(2);

        // Two caps: bottom (h = 0) and top (h = height)
        std::vector<std::pair<float, glm::vec3>> caps = {
            {0.0f, -axis},
            {height, axis}
        };

        for (const auto& [cap_h, cap_normal] : caps) {
            if (const float denominator {glm::dot(ray.get_direction(), cap_normal)}; std::fabs(denominator) > 1e-6f) {
                glm::vec3 cap_center {base_center + axis * cap_h};
                if (const float t {glm::dot(cap_center - ray.get_origin(), cap_normal) / denominator}; t > 0.0f) {
                    // Check if intersection point lies within radius of the cap
                    if (glm::length(ray.at(t) - cap_center) <= radius) {
                        t_caps.push_back(t);
                    }
                }
            }
        }

        t_side.insert(t_side.end(), t_caps.begin(), t_caps.end());
        if (t_side.empty()) return {};
        return t_side;
    }

    // Compute surface normal at point P
    glm::vec3 Cylinder::normal_at(const glm::vec3& P) const {
        const glm::vec3 AP {P - base_center};
        const float h {glm::dot(AP, axis)};

        if (std::abs(h) < 1e-6) return -axis;
        if (std::abs(h - height) < 1e-6) return axis;

        const glm::vec3 axis_point = base_center + h * axis;
        return glm::normalize(P - axis_point);
    }

} // Objects