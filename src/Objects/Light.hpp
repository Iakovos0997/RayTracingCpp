#ifndef RAYTRACINGCPP_SRC_OBJECTS_LIGHT_HPP
#define RAYTRACINGCPP_SRC_OBJECTS_LIGHT_HPP
#include <glm/glm.hpp>
#include <stdexcept>
#include <string>

namespace Objects {
    class Light {
    public:
        enum class Type { Ambient, Point, Directional };

        Light(Type type_, float intensity_)
            : type(type_)
        {
            if (intensity_ < 0.f || intensity_ > 1.f)
                throw std::out_of_range("Light intensity must be between 0 and 1.");
            intensity = intensity_;
        }

        virtual ~Light() = default;

        Type get_type() const { return type; }
        float get_intensity() const { return intensity; }

    protected:
        Type type;
        float intensity;
    };

    // ------------------ Derived Light Types ------------------

    class AmbientLight : public Light {
    public:
        AmbientLight(float intensity_)
            : Light(Type::Ambient, intensity_) {}
    };

    class PointLight : public Light {
    public:
        PointLight(float intensity_, const glm::vec3& position_)
            : Light(Type::Point, intensity_), position(position_) {}

        glm::vec3 get_position() const { return position; }
        void set_position(const glm::vec3& pos) { position = pos; }

    private:
        glm::vec3 position;
    };

    class DirectionalLight : public Light {
    public:
        DirectionalLight(float intensity_, const glm::vec3& direction_)
            : Light(Type::Directional, intensity_), direction(glm::normalize(direction_)) {}

        glm::vec3 get_direction() const { return direction; }
        void set_direction(const glm::vec3& dir) { direction = glm::normalize(dir); }

    private:
        glm::vec3 direction;
    };
}
#endif // RAYTRACINGCPP_SRC_OBJECTS_LIGHT_HPP