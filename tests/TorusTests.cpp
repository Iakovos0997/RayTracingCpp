//
// Created by jimmy on 9/4/25.
//
// tests/test_torus.cpp
#include <algorithm>
#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Objects/Torus.hpp"
#include "Utilities/Ray.hpp"
#include "Utilities/RGB.hpp"
#include "RayTracing/RayTracing.hpp"

using Objects::Torus;

static bool approx_eq(float a, float b, float eps = 1e-4f) {
  return std::abs(a - b) <= eps;
}

static bool approx_vec3(const glm::vec3& a, const glm::vec3& b, float eps = 1e-4f) {
  return glm::all(glm::lessThanEqual(glm::abs(a - b), glm::vec3(eps)));
}

// ---------------------------
// solve_quartic() unit tests
// ---------------------------

TEST(Torus_SolveQuartic, DistinctRealRoots) {
  // (t-1)(t-2)(t-3)(t-4) = t^4 -10 t^3 +35 t^2 -50 t + 24
  const float A = 1.f, B = -10.f, C = 35.f, D = -50.f, E = 24.f;
  auto roots = Torus::solve_quartic(A, B, C, D, E);
  std::ranges::sort(roots);
  ASSERT_EQ(roots.size(), 4u);
  EXPECT_TRUE(approx_eq(roots[0], 1.f));
  EXPECT_TRUE(approx_eq(roots[1], 2.f));
  EXPECT_TRUE(approx_eq(roots[2], 3.f));
  EXPECT_TRUE(approx_eq(roots[3], 4.f));
}

TEST(Torus_SolveQuartic, NoRealRoots) {
  // t^4 + 1 = 0 -> no real roots
  const float A = 1.f, B = 0.f, C = 0.f, D = 0.f, E = 1.f;
  auto roots = Torus::solve_quartic(A, B, C, D, E);
  EXPECT_TRUE(roots.empty());
}

TEST(Torus_SolveQuartic, RepeatedRealRoots) {
  // (t-2)^2 (t+1)^2 = t^4 -2 t^3 -3 t^2 + 4 t + 4
  const float A = 1.f, B = -2.f, C = -3.f, D = 4.f, E = 4.f;
  auto roots = Torus::solve_quartic(A, B, C, D, E);
  std::sort(roots.begin(), roots.end());
  ASSERT_EQ(roots.size(), 4u);
  EXPECT_TRUE(approx_eq(roots[0], -1.f));
  EXPECT_TRUE(approx_eq(roots[1], -1.f));
  EXPECT_TRUE(approx_eq(roots[2],  2.f));
  EXPECT_TRUE(approx_eq(roots[3],  2.f));
}

// ---------------------------------
// intersect() geometric unit tests
// ---------------------------------

TEST(Torus_Intersect, MissAlongAxisLine) {
  // Torus centered at origin, axis = z (Renderables typically default z-axis)
  // R = 2, r = 0.5. A ray along the central z-axis passes through the hole and should MISS.
  Torus torus({0,0,0}, 2.0f, 0.5f);

  Ray ray({0,0,-5}, {0,0,1}); // along +z through (0,0,*) line
  auto roots = torus.intersect(ray);
  EXPECT_TRUE(roots.empty());
}

TEST(Torus_Intersect, HitAcrossTubeTwice) {
  // Same torus. Shoot from +x toward -x through y=z=0 (through the tube cross-section).
  Torus torus({0,0,0}, 2.0f, 0.5f);

  Ray ray({5,0,0}, {-1,0,0});
  auto roots = torus.intersect(ray);

  // Expect two positive intersections (enter/exit). Some solvers may also return negative roots.
  // Filter positive t only and check there are exactly 2, increasing.
  std::vector<float> pos;
  for (auto t : roots) if (t > 0.f) pos.push_back(t);
  std::sort(pos.begin(), pos.end());

  ASSERT_EQ(pos.size(), 2u);
  EXPECT_LT(0.f, pos[0]);
  EXPECT_LT(pos[0], pos[1]);
}

// ---------------------------
// normal_at() unit tests
// ---------------------------

TEST(Torus_NormalAt, UnitLengthAndDirectionAtOuterEquator) {
  // For z-aligned torus at origin, point P = (R + r, 0, 0) lies on the outer equator.
  // Expected normal ~ +x and unit length.
  const float R = 2.0f, r = 0.5f;
  Torus torus({0,0,0}, R, r);

  glm::vec3 P(R + r, 0.f, 0.f);
  glm::vec3 n = torus.normal_at(P);

  // Unit length
  EXPECT_NEAR(glm::length(n), 1.0f, 1e-4);

  // Close to +X direction
  EXPECT_TRUE(approx_vec3(n, glm::vec3(1,0,0), 1e-3f));
}

TEST(Torus_NormalAt, TranslatedTorusSameNormalShape) {
  // Move torus +10 in z; point moves with it; normal should still be ~ +x.
  const float R = 2.0f, r = 0.5f;
  glm::vec3 center(0,0,10);
  Torus torus(center, R, r);

  glm::vec3 P_world = center + glm::vec3(R + r, 0.f, 0.f);
  glm::vec3 n = torus.normal_at(P_world);

  EXPECT_NEAR(glm::length(n), 1.0f, 1e-4);
  EXPECT_TRUE(approx_vec3(n, glm::vec3(1,0,0), 1e-3f));
}

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

TEST(Torus_Render, itRenders) {
    constexpr int width = 600;
    constexpr int height = 600;

    // Create objects
    std::vector<std::shared_ptr<Objects::IRenderable>> objects;
    objects.emplace_back(std::make_shared<Objects::Torus>());

    // Create lights
    std::vector<std::shared_ptr<Objects::Light>> lights;
    lights.emplace_back(std::make_shared<Objects::AmbientLight>(0.2f));
    lights.emplace_back(std::make_shared<Objects::PointLight>(0.6f, glm::vec3(2,3,-2)));
    lights.emplace_back(std::make_shared<Objects::DirectionalLight>(0.2f, glm::vec3(1, 4, 4)));

    // Scene
    const RayTracing::Scene scene(objects, lights);

    // Render
    render_scene(width, height, scene);
}
