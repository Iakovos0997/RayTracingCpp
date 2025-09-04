#ifndef RAYTRACINGCPP_SRC_RAYTRACING_RAYTRACING_HPP
#define RAYTRACINGCPP_SRC_RAYTRACING_RAYTRACING_HPP
#include <vector>
#include <memory>
#include <algorithm>
#include "Utilities/RGB.hpp"
#include "Utilities/Ray.hpp"
#include "Objects/Renderable.hpp"
#include "Objects/Light.hpp"
#include <glm/glm.hpp>

namespace RayTracing {

    class Scene {
        std::vector<std::shared_ptr<Objects::Renderable>> objects;
        std::vector<std::shared_ptr<Objects::Light>> lights;
    public:
        // Constructors
        Scene(
            const std::vector<std::shared_ptr<Objects::Renderable>>& objects_,
            const std::vector<std::shared_ptr<Objects::Light>>& lights_
        ) : objects(objects_), lights(lights_) {}

        // Getters
        const std::vector<std::shared_ptr<Objects::Renderable>>& get_objects() const { return objects; }
        const std::vector<std::shared_ptr<Objects::Light>>& get_lights() const { return lights; }
    };

    glm::vec3 canvas_to_viewport(int x, int y, float Vw, float Vh, float d, int Cw, int Ch);
    RGB trace_ray(const Ray& ray, float t_min, float t_max, const Scene& scene, int depth = 0);
    float compute_lighting(const glm::vec3& P, const glm::vec3& N_in, const std::vector<std::shared_ptr<Objects::Light>>& lights, const glm::vec3& V_in, int shininess);
    void save_ppm_binary(const std::string& filename, const std::vector<RGB>& pixels, int width, int height);
}

#endif // RAYTRACINGCPP_SRC_RAYTRACING_RAYTRACING_HPP