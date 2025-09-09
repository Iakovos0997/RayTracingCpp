#ifndef RAYTRACINGCPP_SRC_OBJECTS_RENDERABLE_HPP
#define RAYTRACINGCPP_SRC_OBJECTS_RENDERABLE_HPP
#include <glm/glm.hpp>
#include <vector>
#include "Utilities/RGB.hpp"
#include "Utilities/Ray.hpp"

namespace Objects {
    class IRenderable {
    protected:
        RGB color;
        int specular;
        float reflectivity; // 0 = non-reflective, 1 = perfect mirror
        glm::vec3 axis; // Axis for rotation
    
    public:
        // Constructor
        explicit IRenderable(const RGB& color_ = RGB(255,0,0), int specular_=500, float reflectivity_=0.0f, const glm::vec3& axis_={1,1,1});

        virtual ~IRenderable() = default;

        // Getters
        RGB get_color() const;
        int get_specular() const;
        float get_reflectivity() const;
        glm::vec3 get_axis() const;

        // Setters
        void set_color(const RGB& color_);
        void set_specular(int specular_);
        void set_reflectivity(float reflectivity_);
        void set_axis(const glm::vec3& axis_);

        // Compute intersection with a ray: O + t*D
        // Returns a list of t values (min t first), or empty vector if no intersection
        virtual std::vector<float> intersect(const Ray& ray) const = 0;

        // Compute surface normal at point P
        virtual glm::vec3 normal_at(const glm::vec3& P) const = 0;
    };
}
#endif // RAYTRACINGCPP_SRC_OBJECTS_RENDERABLE_HPP