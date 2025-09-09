#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

#include "Objects/Cylinder.hpp"
#include "Utilities/RGB.hpp"
#include "Objects/Sphere.hpp"
#include "Objects/Light.hpp"
#include "Objects/Plane.hpp"
#include "RayTracing/RayTracing.hpp"
#include "Objects/Torus.hpp"

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
    std::vector<std::shared_ptr<Objects::IRenderable>> objects;
    objects.emplace_back(std::make_shared<Objects::Sphere>(
        RGB(255, 0, 0), 500, 0.1f, glm::vec3(0,-1,3), 1.0f
    ));
    objects.emplace_back(std::make_shared<Objects::Sphere>(
        RGB(0, 0, 255), 500, 0.1f, glm::vec3(2,0,4), 1.0f
    ));
    objects.emplace_back(std::make_shared<Objects::Sphere>(
        RGB(0, 255, 0), 10, 0.1f, glm::vec3(-2,0,4), 1.0f
    ));
    objects.emplace_back(std::make_shared<Objects::Sphere>(
    RGB(180, 200, 100), 500, 0.0f, glm::vec3(0,0,11), 1.0f
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

    objects.emplace_back(std::make_shared<Objects::Torus>(
        glm::vec3(0, 2.5, 7), 1.5f, 0.5f, RGB(0, 255, 255), 300, 0, glm::vec3(1, -1, 1)
    ));
    // const glm::vec3 &center_, const float &major_radius_, const float &minor_radius_, const RGB &color_, const int &specular_, const float &reflectivity_, const glm::vec3 &axis_

// # Floating torus
//     Torus(
//         center=Vector(0, 2.5, 7),
//         major_radius=1.5,
//         minor_radius=0.5,
//         color=(0, 255, 255),
//         specular=300,
//         axis=Vector(1, -1, 1)
//     )

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