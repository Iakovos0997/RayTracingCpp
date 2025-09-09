//
// Created by jimmy on 8/25/25.
//

#include "Torus.hpp"
#include "Utilities/Math.hpp"
#include <algorithm>
#include <Eigen/Dense>
#include <complex>
#include <vector>
#include <cmath>

namespace Objects {

    using vec3 = glm::vec3;

    Torus::Torus()
        : IRenderable(), center({0, 0, 10}), major_radius(2.0f), minor_radius(0.5f) {}

    Torus::Torus(const glm::vec3 &center_, const float &major_radius_, const float &minor_radius_)
        : IRenderable(), center(center_), major_radius(major_radius_), minor_radius(minor_radius_) {}

    Torus::Torus(const glm::vec3 &center_, const float &major_radius_, const float &minor_radius_, const RGB &color_, const int &specular_, const float &reflectivity_, const glm::vec3 &axis_)
            : IRenderable(color_, specular_, reflectivity_, axis_), center(center_), major_radius(major_radius_), minor_radius(minor_radius_) {
    }

    std::vector<float> Torus::solve_quartic(const float /*A*/, const float B, const float C, const float D, const float E) {

        const auto b {static_cast<double>(B)};
        const auto c {static_cast<double>(C)};
        const auto d {static_cast<double>(D)};
        const auto e {static_cast<double>(E)};

        std::vector<double> rD;
        Math::solve_quartic_monic(b,c,d,e,rD);

        std::vector<float> out;
        out.reserve(rD.size());
        for (const double& t : rD) out.push_back(static_cast<float>(t));
        return out;
    }

    // Build a right-handed orthonormal basis (u, v, w), where w = normalized axis.
    inline void make_orthonormal_basis(const glm::vec3& axis_in, glm::vec3& u, glm::vec3& v, glm::vec3& w) {
        w = axis_in;
        const glm::vec3 pick {(std::abs(axis_in.x) < 0.9f) ? glm::vec3(1,0,0) : glm::vec3(0,1,0)};
        u = glm::normalize(glm::cross(pick, w));
        v = glm::cross(w, u);                    // already unit if u,w are unit & ⟂
    }

std::vector<float> Torus::intersect(const Ray& ray) const {
        // Build Orthonormal basis
        glm::vec3 u, v, w;
        make_orthonormal_basis(axis, u, v, w);

        // Transform Ray into local torus coordinates
        auto O_rel {ray.get_origin() - center};
        auto O_local {vec3(glm::dot(O_rel, u), glm::dot(O_rel, v), glm::dot(O_rel, w))};
        auto D_local {vec3(glm::dot(ray.get_direction(), u), glm::dot(ray.get_direction(), v), glm::dot(ray.get_direction(), w))};

        // Quartic Coefficients
        auto dx {D_local.x};
        auto dy {D_local.y};
        auto dz {D_local.z};

        auto ox {O_local.x};
        auto oy {O_local.y};
        auto oz {O_local.z};

        auto sum_d_sq {dx*dx + dy*dy + dz*dz};
        auto e {ox*ox + oy*oy + oz*oz - major_radius*major_radius - minor_radius*minor_radius};
        auto f {ox*dx + oy*dy + oz*dz};

        auto A {sum_d_sq * sum_d_sq};
        auto B {4.0f * f * sum_d_sq};
        auto C {2.0f * sum_d_sq * e + 4.0f * f * f + 4.0f * major_radius*major_radius * dz*dz};
        auto D {4.0f  * f * e + 8 * major_radius * major_radius * oz * dz};
        auto E {e*e - 4.0f * major_radius * major_radius * (minor_radius*minor_radius - oz*oz)};

        auto roots {solve_quartic(A, B, C, D, E)};
        std::ranges::sort(roots);
        return roots;
}

    glm::vec3 Torus::normal_at(const glm::vec3& P) const {
        const glm::vec3 P_rel {P - center};

        glm::vec3 u, v, w;
        make_orthonormal_basis(axis, u, v, w);

        const float x {glm::dot(P_rel, u)};
        const float y {glm::dot(P_rel, v)};
        const float z {glm::dot(P_rel, w)};

        // Local-space normal on torus aligned to z-axis:
        double len_xy = std::sqrt(x*x + y*y);
        double mx = (len_xy == 0.0) ? major_radius : x * major_radius / len_xy;
        double my = (len_xy == 0.0) ? 0.0          : y * major_radius / len_xy;

        const glm::vec3 N_local {glm::normalize(glm::vec3(x - mx, y - my, z))};
        const glm::vec3 N_world {glm::normalize(u * N_local.x + v * N_local.y + w * N_local.z)};
        return N_world;
    }
} // Objects