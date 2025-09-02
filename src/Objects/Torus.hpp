//
// Created by jimmy on 8/25/25.
//

#ifndef RAYTRACINGCPP_TORUS_H
#define RAYTRACINGCPP_TORUS_H
#include <Eigen/Dense>
#include <complex>
#include <vector>
#include "Renderable.hpp"


namespace Objects {
    class Torus : public Renderable {
        glm::vec3 center, axis;
        float minor_radius, major_radius;

    public:
        static std::vector<float> solve_quartic_real_positive(float A, float B, float C, float D, float E, float eps = 1e-6f);
        std::vector<float> intersect(const Ray& ray) const override;
        glm::vec3 normal_at(const glm::vec3& P) const override;
    };
} // Objects

#endif //RAYTRACINGCPP_TORUS_H