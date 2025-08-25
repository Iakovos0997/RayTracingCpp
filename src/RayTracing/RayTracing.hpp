#ifndef RAYTRACINGCPP_SRC_RAYTRACING_RAYTRACING_HPP
#define RAYTRACINGCPP_SRC_RAYTRACING_RAYTRACING_HPP
#include <vector>
#include <memory>
#include <algorithm>
#include "RGB.hpp"
#include "Ray.hpp"
#include "Renderable.hpp"
#include "Light.hpp"
#include <glm/glm.hpp>

namespace RayTracing {

    struct Scene {
        std::vector<std::shared_ptr<Renderable>> objects;
        std::vector<std::shared_ptr<Light>> lights;

        Scene(
            const std::vector<std::shared_ptr<Renderable>>& objects_,
            const std::vector<std::shared_ptr<Light>>& lights_
        ) : objects(objects_), lights(lights_) {}
    };

    glm::vec3 canvas_to_viewport(int x, int y, float Vw, float Vh, float d, int Cw, int Ch);
    RGB trace_ray(const Ray& ray, float t_min, float t_max, const Scene& scene, int depth);
    float compute_lighting(const glm::vec3& P, const glm::vec3& N, const Scene& scene, const glm::vec3& V, int s);
    void save_ppm_binary(const std::string& filename, const std::vector<RGB>& pixels, int width, int height);
}

#endif // RAYTRACINGCPP_SRC_RAYTRACING_RAYTRACING_HPP