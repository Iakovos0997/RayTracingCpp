//
// Created by jimmy on 9/5/25.
//

#ifndef RAYTRACER_MATH_H
#define RAYTRACER_MATH_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

/// Numeric utilities and robust polynomial helpers for the ray tracer.
/// Header-only: all non-template functions are defined `inline` below.
namespace Math {

    // ---------------------------------------------------------------------
    // Tolerances
    // ---------------------------------------------------------------------
    struct Eps {
        static constexpr double general   {1e-12}; // small epsilon
        static constexpr double sqrt_arg  {1e-14}; // tolerance for negative -> 0 under sqrt
        static constexpr double residual  {1e-10}; // accept root if |f(x)| <= residual
        static constexpr double merge     {1e-6};  // collapse near-duplicates
        static constexpr double deriv_min {1e-14}; // guard for Newton derivative
    };

    // ---------------------------------------------------------------------
    // Basics
    // ---------------------------------------------------------------------

    /// Clamp a value into [lo, hi] (UB if lo > hi).
    template <class T>
    constexpr T clamp(T x, T lo, T hi) {
        return x < lo ? lo : (x > hi ? hi : x);
    }

    /// Clamp to [-1, 1] (useful before acos()).
    inline double clamp_unit(const double& x) {
        return clamp(x, static_cast<double>(-1), static_cast<double>(1));
    }

    /// Relative near-equality: |a-b| <= eps * max(1, |a|, |b|)
    inline bool nearly_equal(const double& a, const double& b, const double eps = Eps::merge) {
        const double m {std::max({static_cast<double>(1), std::abs(a), std::abs(b)})};
        return std::abs(a - b) <= eps * m;
    }

    /// Linear interpolation: (1-t)*a + t*b (no clamping on t).
    template <class T>
    constexpr T lerp(const T& a, const T& b, double t) {
        return T((1 - t) * a + t * b);
    }

    // ---------------------------------------------------------------------
    // Safe elementary ops
    // ---------------------------------------------------------------------

    /// sqrt with tiny-negative clamp to zero; returns NaN if truly negative.
    inline double safe_sqrt(double x) {
        if (x < 0 && x > -Eps::sqrt_arg) x = 0;
        return x < 0 ? std::numeric_limits<double>::quiet_NaN() : std::sqrt(x);
    }

    /// acos with input clamped to [-1, 1] to avoid NaNs from tiny overshoots.
    inline double safe_acos(const double& x) {
        return std::acos(clamp_unit(x));
    }

    // ---------------------------------------------------------------------
    // Monic quartic (x^4 + b x^3 + c x^2 + d x + e)
    // ---------------------------------------------------------------------

    /// Horner evaluation of monic quartic.
    inline double horner4_monic(const double& b, const double& c,
                                const double& d, const double& e,
                                const double& x) {
    #if defined(__FAST_MATH__) || defined(__FMA__)
        double y {std::fma(1.0, x, b)};
        y = std::fma(y, x, c);
        y = std::fma(y, x, d);
        y = std::fma(y, x, e);
        return y;
    #else
        return ((((x + b) * x + c) * x + d) * x + e);
    #endif
    }

    /// Derivative of monic quartic: 4x^3 + 3b x^2 + 2c x + d
    inline double d_horner4_monic(const double& b, const double& c,
                                  const double& d, const double& /*e*/,
                                  const double& x) {
    #if defined(__FAST_MATH__) || defined(__FMA__)
        double y = std::fma(4.0, x, 3.0 * b);
        y = std::fma(y, x, 2.0 * c);
        y = std::fma(y, x, d);
        return y;
    #else
        return ((4.0 * x + 3.0 * b) * x + 2.0 * c) * x + d;
    #endif
    }

    // ---------------------------------------------------------------------
    // Newton polish
    // ---------------------------------------------------------------------

    /// 1–2 Newton steps to tighten a root estimate; guarded derivative.
    inline double newton_polish_quartic_monic(const double& b, const double& c,
                                              const double& d, const double& e,
                                              double t) {
        for (int it = 0; it < 2; ++it) {
            const double f  {horner4_monic(b, c, d, e, t)};
            const double fp {d_horner4_monic(b, c, d, e, t)};
            if (std::abs(fp) < Eps::deriv_min) break;
            t -= f / fp;
        }
        return t;
    }

    // ---------------------------------------------------------------------
    // Ferrari solver (monic) — fills all distinct real roots (ascending)
    // ---------------------------------------------------------------------

    /// Solve x^4 + b x^3 + c x^2 + d x + e = 0 (monic) via robust Ferrari/Cardano path.
    inline void solve_quartic_monic(const double& b, const double& c,
                                    const double& d, const double& e,
                                    std::vector<double>& out_roots) {
        out_roots.clear();

        // Precompute
        const double b2 {b*b}, b3 {b2*b};
        const double c2 {c*c}, c3 {c2*c};
        const double bd {b*d}, bcd {bd*c}, b2e {b2*e}, d2 {d*d}, ce {c*e}, bc {b*c};
        const double mbd4 {-0.25 * b};

        // Depressed quartic params (x = y - b/4)
        const double p {(8.0 * c - 3.0 * b2) / 8.0};
        const double q {(b3 - 4.0 * bc + 8.0 * d) / 8.0};

        // Resolvent
        const double t0 {c2 - 3.0 * bd + 12.0 * e};
        const double t1 {2.0 * c3 - 9.0 * bcd + 27.0 * b2e + 27.0 * d2 - 72.0 * ce};
        const double disc {t1 * t1 - 4.0 * t0 * t0 * t0};

        double sint {0.0};
        if (disc < 0.0 && t0 > 0.0) {
            const double st0 {std::sqrt(std::max(0.0, t0))};
            const double phi {safe_acos(t1 / (2.0 * t0 * st0)) / 3.0};
            sint = (-2.0/3.0) * p + (2.0/3.0) * st0 * std::cos(phi);
        } else {
            const double sq   {std::sqrt(std::max(0.0, disc))};
            double bigq       {std::cbrt(0.5 * (t1 + sq))};
            if (std::abs(bigq) < Eps::general) bigq = std::cbrt(0.5 * (t1 - sq));
            const double inv  {(std::abs(bigq) < Eps::general) ? 0.0 : (t0 / bigq)};
            sint = (-2.0/3.0) * p + (1.0/3.0) * (bigq + inv);
        }

        double sint_c {sint};
        if (sint_c < 0.0 && sint_c > -Eps::sqrt_arg) sint_c = 0.0;
        if (sint_c < 0.0) return;

        const double s {0.5 * std::sqrt(sint_c)};
        if (std::abs(s) < Eps::general) return;

        const double rootint {-(sint + 2.0 * p)};
        const double qds     {q / s};

        auto add_roots = [&](double rad, const double shift) {
            if (rad < 0.0 && rad > -Eps::sqrt_arg) rad = 0.0;
            if (rad >= 0.0) {
                const double r = 0.5 * std::sqrt(std::max(0.0, rad));
                out_roots.push_back(shift + r);
                out_roots.push_back(shift - r);
            }
        };
        add_roots(rootint + qds, mbd4 - s);
        add_roots(rootint - qds, mbd4 + s);

        if (out_roots.empty()) return;

        // Polish + residual filter
        std::vector<double> acc;
        acc.reserve(out_roots.size());
        for (double t : out_roots) {
            t = newton_polish_quartic_monic(b, c, d, e, t);
            if (const double res {std::abs(horner4_monic(b, c, d, e, t))}; std::isfinite(t) && res <= Eps::residual) acc.push_back(t);
        }
        if (acc.empty()) { out_roots.clear(); return; }

        std::ranges::sort(acc);
        out_roots.clear();
        out_roots.reserve(acc.size());
        for (double x : acc) {
            if (out_roots.empty() || !nearly_equal(x, out_roots.back(), Eps::merge))
                out_roots.push_back(x);
        }
    }

    // ---------------------------------------------------------------------
    // Orthonormal basis (ONB)
    // ---------------------------------------------------------------------

    /// Build a right-handed ONB given an axis type with .x/.y/.z (e.g., glm::vec3).
    template <class Vec3, class Cross, class Dot, class Normalize>
    inline void make_onb(const Vec3& axis, Vec3& u, Vec3& v, Vec3& w,
                         Cross cross, Dot /*dot*/, Normalize normalize) {
        w = axis;
        const Vec3 pick = (std::abs(axis.x) < 0.9f) ? Vec3{1,0,0} : Vec3{0,1,0};
        u = normalize(cross(pick, w));
        v = cross(w, u); // u,w unit & orthogonal -> v unit
    }

} // namespace Math

#endif // RAYTRACER_MATH_H
