#include "IRenderable.hpp"

namespace Objects {
    // Constructor
    IRenderable::IRenderable(const RGB& color_, const int specular_, const float reflectivity_, const glm::vec3& axis_)
        : color(color_), specular(specular_), reflectivity(reflectivity_), axis(glm::normalize(axis_)) {}

    // Getters
    RGB IRenderable::get_color() const { return color; }
    int IRenderable::get_specular() const { return specular; }
    float IRenderable::get_reflectivity() const { return reflectivity; }
    glm::vec3 IRenderable::get_axis() const { return axis; }

    // Setters
    void IRenderable::set_color(const RGB& color_) { color = color_; }
    void IRenderable::set_specular(const int specular_) { specular = specular_; }
    void IRenderable::set_reflectivity(const float reflectivity_) { reflectivity = reflectivity_; }
    void IRenderable::set_axis(const glm::vec3& axis_) { axis = glm::normalize(axis_); }
}