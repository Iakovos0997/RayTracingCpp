#include "RayTracing.hpp"
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>

#include <utility>
namespace RayTracing {

    using namespace glm;

    // -----------------------------------------------------------------------------
    // Constants / configuration
    // -----------------------------------------------------------------------------

    static constexpr float EPS                  {1e-4};
    static const RGB BLACK                      {0.0f, 0.0f, 0.0f};
    static const RGB BACKGROUND_COLOR           {255, 255, 255};    // White background
    static constexpr int MAX_RECURSION_DEPTH    {3};                // Max recursion for reflections

    // -----------------------------------------------------------------------------
    // Canvas → Viewport mapping
    // -----------------------------------------------------------------------------

    /**
     * @brief Map integer canvas coordinates (pixels) to a viewport-space direction.
     *
     * @param x   Canvas X in pixels, centered at 0 (i.e., [-Cw/2, Cw/2]).
     * @param y   Canvas Y in pixels, centered at 0 (i.e., [ Ch/2,-Ch/2 ]).
     * @param Vw  Viewport width in world units.
     * @param Vh  Viewport height in world units.
     * @param d   Distance from camera to projection plane.
     * @param Cw  Canvas width in pixels.
     * @param Ch  Canvas height in pixels.
     * @return    3D vector in viewport space (z = d).
     */
    vec3 canvas_to_viewport(const int x, const int y, const float Vw, const float Vh, const float d, const int Cw, const int Ch) {
        return {x * Vw / Cw, y * Vh / Ch, d};
    }

    // -----------------------------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------------------------

    /**
     * @brief Check if a glm::vec3 is normalized (|v| ≈ 1).
     *
     * @param v        Vector to test.
     * @param epsilon  Tolerance for floating-point error.
     * @return true if |v| is approximately 1, false otherwise.
     */
    inline bool is_normalized(const glm::vec3& v, const float epsilon = 1e-6f) {
        return std::fabs(length(v) - 1.0f) <= epsilon;
    }

    inline void closest_interaction(const Ray& ray, const float& t_min, const float& t_max, const Scene& scene, float& closest_t, std::shared_ptr<Objects::Renderable>& closest_object) {
            closest_t = INFINITY;
            closest_object = nullptr;

        for (const auto& object : scene.get_objects()) {
            for (const std::vector<float> all_ts {object->intersect(ray)}; const auto& t : all_ts) {
                if (t > t_min && t < t_max && t < closest_t) {
                    closest_t = t;
                    closest_object = object;
                }
            }
        }
    }
    // -----------------------------------------------------------------------------
    // Ray tracing core
    // -----------------------------------------------------------------------------

    /**
     * @brief Trace a ray through the scene and compute its resulting color.
     *
     * Finds the nearest intersection in [t_min, t_max], shades the hit point using
     * Phong lighting, and—if the material is reflective—recursively traces a
     * reflected ray (up to MAX_RECURSION_DEPTH).
     *
     * @param ray     Primary or secondary ray (origin + direction).
     * @param t_min   Lower bound for valid intersections (e.g., small epsilon).
     * @param t_max   Upper bound for valid intersections (e.g., infinity).
     * @param scene   Scene containing renderables and lights.
     * @param depth   Current recursion depth (0 for primaries).
     * @return        RGB color for the ray.
     */
    RGB trace_ray(const Ray& ray, float t_min, float t_max, const Scene& scene, const int depth) {
        typedef std::shared_ptr<Objects::Renderable> ObjectPtr;
        if (depth > MAX_RECURSION_DEPTH) {
            return BLACK;
        }

        // Find the closest intersection
        float closest_t {INFINITY};
        ObjectPtr closest_object {nullptr};

        closest_interaction(ray, t_min, t_max, scene, closest_t, closest_object);

        // No hit: return background
        if (closest_object == nullptr) {
            return BACKGROUND_COLOR;
        }

        // ----- Shading basis vectors / point -----
        const vec3 P {ray.at(closest_t)};                  // intersection point
        const vec3 N {closest_object->normal_at(P)};       // surface normal at P
        const vec3 V {-ray.get_direction()};               // view vector (toward camera)

        // ----- Local shading (diffuse + specular) -----
        const float intensity {compute_lighting(P, N, scene.get_lights(), V, closest_object->get_specular())};
        RGB local_color {closest_object->get_color() * intensity};

        // ----- Reflections -----
        if (float reflectivity {closest_object->get_reflectivity()}; reflectivity > 0) {
            const vec3 R {normalize(reflect(ray.get_direction(), N))};
            const Ray reflected_ray {P + R * EPS, R};
            const RGB reflected_color {trace_ray(reflected_ray, EPS, t_max, scene, depth + 1)};
            local_color = local_color * (1.0f - reflectivity) + reflected_color * reflectivity;
        }

        return local_color;
    }

    /**
     * @brief Compute Phong lighting at a point.
     *
     * Accumulates ambient, diffuse, and (optionally) specular terms from all lights.
     *
     * @param P         World-space point being shaded.
     * @param N_in      Surface normal at P (may or may not be normalized).
     * @param lights    Scene lights.
     * @param V_in      View vector (from P toward the eye; may or may not be normalized).
     * @param shininess Phong specular exponent (<= 0 disables specular).
     * @return          Total light intensity in [0, 1].
     */
    float compute_lighting(const vec3& P, const vec3& N_in, const std::vector<std::shared_ptr<Objects::Light>>& lights, const vec3& V_in, const int shininess) {

        // Normalize inputs if needed
        const vec3 N {is_normalized(N_in) ? N_in : normalize(N_in)};
        const vec3 V {is_normalized(V_in) ? V_in : normalize(V_in)};
        float intensity = 0.0f;

        for (const auto& light : lights) {

            // Ambient contribution
            if (light->get_type() == Objects::Light::Type::Ambient) {
                intensity += light->get_intensity();
                continue;
            }

            // Direction from P toward the light
            vec3 L;

            if (light->get_type() == Objects::Light::Type::Point) {
                if (const auto pt {std::dynamic_pointer_cast<Objects::PointLight>(light)}) {
                    L = normalize(pt->get_position() - P);
                }
            } else { // Directional
                if (const auto dl {std::dynamic_pointer_cast<Objects::DirectionalLight>(light)}) {
                    L = normalize(dl->get_direction());
                }
            }

            // Diffuse: max(0, N·L)
            const float n_dot_l {dot(N, L)};
            if (n_dot_l > 0) {
                intensity += light->get_intensity() * n_dot_l;
            }

            // Specular (Phong): max(0, R·V)^s
            if (shininess != -1) {
                const vec3 R {N * 2.0f * n_dot_l - L};
                if (const float r_dot_v {dot(R, V)}; r_dot_v > 0.0f) {
                    intensity += light->get_intensity() * std::pow(r_dot_v, shininess);
                }
            }
        }

        // Clamp to [0,1] for safety
        return std::clamp(intensity, 0.0f, 1.0f);
    }

    // -----------------------------------------------------------------------------
    // PPM writer (P6 / binary)
    // -----------------------------------------------------------------------------

    /**
     * @brief Write a P6 (binary) PPM image from an RGB framebuffer.
     *
     * Assumes RGB components are in [0, 255]. Values are clamped before writing.
     *
     * @param filename  Output file path.
     * @param pixels    Framebuffer of size width*height in row-major order.
     * @param width     Image width in pixels.
     * @param height    Image height in pixels.
     */
    void save_ppm_binary(const std::string& filename, const std::vector<RGB>& pixels, const int width, const int height) {

        // Ensure all pixel vector is the same size as the window
        if (pixels.size() != width * height) {throw;}

        std::ofstream ofs(filename, std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";

        for (const auto& pixel : pixels) {
            unsigned char r {static_cast<unsigned char>(std::clamp(pixel.r, 0, 255))};
            unsigned char g {static_cast<unsigned char>(std::clamp(pixel.g, 0, 255))};
            unsigned char b {static_cast<unsigned char>(std::clamp(pixel.b, 0, 255))};

            ofs.write(reinterpret_cast<char*>(&r), 1);
            ofs.write(reinterpret_cast<char*>(&g), 1);
            ofs.write(reinterpret_cast<char*>(&b), 1);
        }
        ofs.close();
    }
}