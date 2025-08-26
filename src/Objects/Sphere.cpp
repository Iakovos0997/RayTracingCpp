#include "Sphere.hpp"
namespace Objects {
    // ------------------------
    // Constructors
    // ------------------------
    Sphere::Sphere(const RGB& color_, const int specular_, const float reflectivity_, const glm::vec3& center_, const float radius_)
        : Renderable(color_, specular_, reflectivity_, glm::vec3(0, 1,0)), center(center_), radius(radius_), color_(color_) {}

    // ------------------------
    // Getters
    // ------------------------
    glm::vec3 Sphere::get_center() const { return center; }
    float Sphere::get_radius() const { return radius; }

    // ------------------------
    // Setters
    // ------------------------
    void Sphere::set_center(const glm::vec3& center_) { center = center_; }
    void Sphere::set_radius(const float radius_) { radius = radius_; }

    // ------------------------
    // Override Renderable Methods
    // ------------------------

    // Intersect ray with sphere and return valid t-values
    std::vector<float> Sphere::intersect(const Ray& ray) const {
        // Vector from ray origin to sphere center
        const glm::vec3 OC = ray.get_origin() - center;

        // Quadratic coefficients (a=1 since ray direction is normalized)
        const float b = 2.0f * glm::dot(OC, ray.get_direction());
        const float c = glm::dot(OC, OC) - radius * radius;

        const float discriminant = b * b - 4.0f * c;
        if (discriminant < 0) return {};  // No intersection

        const float sqrt_disc = std::sqrt(discriminant);
        float t1 = (-b - sqrt_disc) / 2.0f;
        float t2 = (-b + sqrt_disc) / 2.0f;

        // Only return positive intersections (in front of the ray)
        std::vector<float> result;
        if (t1 > 0) result.emplace_back(t1);
        if (t2 > 0) result.emplace_back(t2);

        return result;
    }

    // Compute normal at point P on the sphere
    glm::vec3 Sphere::normal_at(const glm::vec3& P) const {
        return glm::normalize(P - this->center);
    }
}