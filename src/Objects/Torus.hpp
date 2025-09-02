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
        glm::vec3 center;
        float major_radius, minor_radius;
        glm::vec3 u_, v_, w_;     // basis
        glm::mat3 world_to_local_; // rows or columns consistent with your use
        float R2_, r2_, fourR2_, eightR2_;
        void rebuild_cache();

    public:
        Torus();
        Torus(const glm::vec3 &center_, const float &major_radius_, const float &minor_radius_);
        Torus(const glm::vec3 &center_, const float &major_radius_, const float &minor_radius_, const RGB &color_, const int &specular_, const float &reflectivity_, const glm::vec3 &axis_);

        static std::vector<float> solve_quartic(float A, float B, float C, float D, float E);
        std::vector<float> intersect(const Ray& ray) const override;
        glm::vec3 normal_at(const glm::vec3& P) const override;
    };
} // Objects

#endif //RAYTRACINGCPP_TORUS_H