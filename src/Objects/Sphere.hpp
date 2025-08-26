#ifndef RAYTRACINGCPP_SRC_OBJECTS_SPHERE_HPP
#define RAYTRACINGCPP_SRC_OBJECTS_SPHERE_HPP
#include <glm/glm.hpp>
#include <vector>
#include "RGB.hpp"
#include "Renderable.hpp"
#include "Ray.hpp"

namespace Objects {
    class Sphere : public Renderable {
        glm::vec3 center;
        float radius;
        const RGB &color_;

    public:
        // Constructor
        explicit Sphere(
        const RGB& color_ = RGB(255, 0, 0),
        int specular_ = 0,
        float reflectivity_ = 0.0f,
        const glm::vec3& center_ = glm::vec3(0, 0, 10),
        float radius_ = 1);

        // Getters
        glm::vec3 get_center() const;
        float get_radius() const;

        // Setters
        void set_center(const glm::vec3& center_);
        void set_radius(float radius_);

        // Override methods from Renderable
        std::vector<float> intersect(const Ray& ray) const override;
        glm::vec3 normal_at(const glm::vec3& P) const override;
    };
}

#endif // RAYTRACINGCPP_SRC_OBJECTS_SPHERE_HPP