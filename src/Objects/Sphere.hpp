#ifndef RAYTRACINGCPP_SRC_OBJECTS_SPHERE_HPP
#define RAYTRACINGCPP_SRC_OBJECTS_SPHERE_HPP
#include <glm/glm.hpp>
#include <vector>
#include "RGB.hpp"
#include "Renderable.hpp"
#include "Ray.hpp"

class Sphere : public Renderable {
    glm::vec3 center;
    float radius;

public:
    // Constructor
    Sphere();
    Sphere(
        const RGB& color_,
        int specular_, 
        float reflectivity_,
        const glm::vec3& axis_,
        const glm::vec3& center_,
        float radius_
    );

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



#endif // RAYTRACINGCPP_SRC_OBJECTS_SPHERE_HPP