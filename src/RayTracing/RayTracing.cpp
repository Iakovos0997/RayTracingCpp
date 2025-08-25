#include "RayTracing.hpp"
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>


namespace RayTracing {

    using namespace glm;

    static const RGB BLACK                  {0, 0, 0};          // Black color
    static const RGB BACKGROUND_COLOR       {255, 255, 255};    // White background
    static const int MAX_RECURSION_DEPTH    {3};                // Max recursion for reflections

    /**
 * @brief Check if a glm::vec3 is normalized (length ~ 1.0).
 *
 * @param v The vector to test.
 * @param epsilon Tolerance for floating-point error (default = 1e-6).
 * @return true if |v| is approximately 1, false otherwise.
 */
    inline bool is_normalized(const glm::vec3& v, float epsilon = 1e-6f) {
        float len = glm::length(v);
        return std::fabs(len - 1.0f) <= epsilon;
    }

    vec3 canvas_to_viewport(int x, int y, float Vw, float Vh, float d, int Cw, int Ch) {
        return vec3(
            x * Vw / Cw,
            y * Vh / Ch,
            d
        );
    }

    RGB trace_ray(const Ray& ray, float t_min, float t_max, const Scene& scene, int depth = 0) {
        if (depth > MAX_RECURSION_DEPTH) {
            return BACKGROUND_COLOR;
        }

        // Find the closest intersection
        float closest_t {INFINITY};
        std::shared_ptr<Renderable> closest_object {nullptr};

        for (const auto& object : scene.objects) {
            auto t_values = object->intersect(ray);
            
            if (t_values.empty()) continue;
            
            for (float t : t_values) {
                if (t >= t_min && t <= t_max && t < closest_t) {
                    closest_t = t;
                    closest_object = object;
                }
            }
        }
        
        if (closest_object == nullptr) {
            return BACKGROUND_COLOR; // Background color (black)
        }

        // Compute Intersection Point between object and Ray
        vec3 P {ray.at(closest_t)};

        // Compute surface Normal of the object at point P
        vec3 N {closest_object->normal_at(P)};

        // Compute view vector (pointing towards the camera)
        vec3 V {normalize(-ray.get_direction())};

        // Local lighting (difuse + specular)
        float intensity {compute_lighting(P, N, scene, V, closest_object->get_specular())};
        RGB local_color {closest_object->get_color() * intensity};

        // Handle reflections
        float reflectivity {closest_object->get_reflectivity()};
        if (reflectivity > 0) {
            vec3 R {normalize(reflect(ray.get_direction(), N))};

            // Small bias to avoid self-intersection
            vec3 bias {N * 1e-4f};
            Ray reflected_ray {P + bias, R};
            RGB reflected_color {trace_ray(reflected_ray, 1e-4f, INFINITY, scene, depth + 1)};
            local_color = local_color * (1.0f - closest_object->get_reflectivity()) + reflected_color * closest_object->get_reflectivity();
        }

        return local_color;
    }

    /**
     * @brief Compute Phong lighting at point P with normal N and view vector V.
     *
     * Accumulates ambient, diffuse, and (optionally) specular contributions
     * from all lights in the scene.
     *
     * @param P         World-space point being shaded.
     * @param N_in      Surface normal at P (not assumed normalized).
     * @param scene     Scene containing lights.
     * @param V_in      View vector (from P toward eye; not assumed normalized).
     * @param shininess Phong specular exponent (<=0 disables specular).
     * @return          Total light intensity in [0,1].
     */
    float compute_lighting(const vec3& P, const vec3& N_in, const Scene& scene, const vec3& V_in, int shininess) {
        // Ensure unit vectors for stable dot/reflect computations
        const vec3 N = normalize(N_in);
        const vec3 V = normalize(V_in);

        float intensity = 0.0f;

        for (const auto& light : scene.lights) {

            // Ambient: add and continue
            if (light->get_type() == Light::Type::Ambient) {
                intensity += light->get_intensity();
                continue;
            }

            // Direction from P toward the light
            vec3 L;

            if (light->get_type() == Light::Type::Point) {
                if (auto pt = std::dynamic_pointer_cast<PointLight>(light)) {
                    L = normalize(pt->get_position() - P);
                }
            } else { // Directional
                if (auto dl = std::dynamic_pointer_cast<DirectionalLight>(light)) {
                    // If your DirectionalLight::get_direction() stores the travel direction,
                    // use the opposite here: L = normalize(-dl->get_direction());
                    L = normalize(dl->get_direction());
                }
            }

            // Diffuse: max(0, N·L)
            const float n_dot_l = dot(N, L);
            if (n_dot_l > 0) {
                intensity += light->get_intensity() * n_dot_l / length(N) * length(L);
            }

            // Specular (Phong): max(0, R·V)^s
            if (shininess > 0) {
                // Incident vector for reflection is -L (from light into surface)
                const vec3 R {normalize(reflect(-L, N))};
                const float r_dot_v {dot(R, V)};
                if (r_dot_v > 0.0f) {
                    intensity += light->get_intensity() * std::pow(r_dot_v, shininess);
                }
            }
        }

        // Clamp for safety
        return std::clamp(intensity, 0.0f, 1.0f);
    }

    void save_ppm_binary(const std::string& filename, const std::vector<RGB>& pixels, const int width, const int height) {
        std::ofstream ofs(filename, std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";

        for (const auto& pixel : pixels) {
            unsigned char r = static_cast<unsigned char>(std::clamp(pixel.r, 0, 255));
            unsigned char g = static_cast<unsigned char>(std::clamp(pixel.g, 0, 255));
            unsigned char b = static_cast<unsigned char>(std::clamp(pixel.b, 0, 255));

            ofs.write(reinterpret_cast<char*>(&r), 1);
            ofs.write(reinterpret_cast<char*>(&g), 1);
            ofs.write(reinterpret_cast<char*>(&b), 1);
        }
        ofs.close();
    }
}