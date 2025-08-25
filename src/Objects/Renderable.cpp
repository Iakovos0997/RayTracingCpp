#include "Renderable.hpp"

// Constructor
Renderable::Renderable(
    const RGB& color_ = RGB(255, 255, 255), 
    int specular_ = 500, 
    float reflectivity_ = 0.0f, 
    const glm::vec3& axis_ = glm::vec3(0, 1, 0)
)
    : color(color_), specular(specular_), reflectivity(reflectivity_), axis(glm::normalize(axis_)) {}

// Getters
RGB Renderable::get_color() const { return color; }
int Renderable::get_specular() const { return specular; }
float Renderable::get_reflectivity() const { return reflectivity; }
glm::vec3 Renderable::get_axis() const { return axis; }

// Setters
void Renderable::set_color(const RGB& color_) { color = color_; }
void Renderable::set_specular(int specular_) { specular = specular_; }
void Renderable::set_reflectivity(float reflectivity_) { reflectivity = reflectivity_; }
void Renderable::set_axis(const glm::vec3& axis_) { axis = glm::normalize(axis_); }