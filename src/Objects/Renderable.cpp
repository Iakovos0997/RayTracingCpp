#include "Renderable.hpp"

namespace Objects {
    // Constructor
    Renderable::Renderable(const RGB& color_, const int specular_, const float reflectivity_, const glm::vec3& axis_)
        : color(color_), specular(specular_), reflectivity(reflectivity_), axis(glm::normalize(axis_)) {}

    // Getters
    RGB Renderable::get_color() const { return color; }
    int Renderable::get_specular() const { return specular; }
    float Renderable::get_reflectivity() const { return reflectivity; }
    glm::vec3 Renderable::get_axis() const { return axis; }

    // Setters
    void Renderable::set_color(const RGB& color_) { color = color_; }
    void Renderable::set_specular(const int specular_) { specular = specular_; }
    void Renderable::set_reflectivity(const float reflectivity_) { reflectivity = reflectivity_; }
    void Renderable::set_axis(const glm::vec3& axis_) { axis = glm::normalize(axis_); }
}