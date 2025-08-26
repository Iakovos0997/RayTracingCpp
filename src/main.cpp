#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

#include "Cylinder.h"
#include "RGB.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include "Plane.h"
#include "RayTracing.hpp"

void render_scene(const int width, const int height, const RayTracing::Scene& scene) {
    constexpr glm::vec3 origin(0, 0, 0);
    std::vector<RGB> framebuffer;
    framebuffer.reserve(width * height);

    for (int y {0}; y < height; ++y) {
        const int y_canvas {height / 2 - y};
        for (int x {0}; x < width; ++x) {
            constexpr float d {1.0f};
            constexpr float Vh {1.0f};
            constexpr float Vw {1.0f};
            const int x_canvas {x - width / 2};
            glm::vec3 direction {glm::normalize(RayTracing::canvas_to_viewport(x_canvas, y_canvas, Vw, Vh, d, width, height))};
            framebuffer.emplace_back(RayTracing::trace_ray(Ray(origin, direction), 1.0f, INFINITY, scene, 0));
        }
    }

    RayTracing::save_ppm_binary("output.ppm", framebuffer, width, height);
    std::cout << "Render complete! Saved to output.ppm\n";
}

int main() {
    constexpr int width = 600;
    constexpr int height = 600;

    // Create objects
    std::vector<std::shared_ptr<Objects::Renderable>> objects;
    objects.emplace_back(std::make_shared<Objects::Sphere>(
        RGB(255, 0, 0), 500, 0.1f, glm::vec3(0,-1,3), 1.0f
    ));
    objects.emplace_back(std::make_shared<Objects::Sphere>(
        RGB(0, 0, 255), 500, 0.1f, glm::vec3(2,0,4), 1.0f
    ));
    objects.emplace_back(std::make_shared<Objects::Sphere>(
        RGB(0, 255, 0), 10, 0.1f, glm::vec3(-2,0,4), 1.0f
    ));

    // Floor plane
    objects.emplace_back(std::make_shared<Objects::Plane>(
        RGB(200,200,200), 100, 0, glm::vec3(0,1,0), glm::vec3(0,-2,0)
        ));

    // Back Mirror plane
    objects.emplace_back(std::make_shared<Objects::Plane>(
    RGB(180,180,200), 500, 0.8f, glm::vec3(0,0,-1), glm::vec3(0,0,13)
    ));

    objects.emplace_back(std::make_shared<Objects::Cylinder>(
        glm::vec3{-1,3,7}, 0.5f, 4, RGB{255, 0, 255}, 500, 0, glm::vec3{1, -1, 1}
        ));

    // Create lights
    std::vector<std::shared_ptr<Objects::Light>> lights;
    lights.emplace_back(std::make_shared<Objects::AmbientLight>(0.2f));
    lights.emplace_back(std::make_shared<Objects::PointLight>(0.6f, glm::vec3(2,3,-2)));
    lights.emplace_back(std::make_shared<Objects::DirectionalLight>(0.2f, glm::vec3(1, 4, 4)));

    // Scene
    const RayTracing::Scene scene(objects, lights);

    // Render
    render_scene(width, height, scene);
    return 0;
}