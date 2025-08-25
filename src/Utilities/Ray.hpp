#ifndef RAYTRACINGCPP_SRC_UTILITIES_RAY_HPP
#define RAYTRACINGCPP_SRC_UTILITIES_RAY_HPP
#include <glm/glm.hpp>

class Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    public:
    Ray(const glm::vec3& o, const glm::vec3& d) 
        : origin(o), direction(glm::normalize(d)) {}
    

    glm::vec3 get_origin() const { return origin; }
    glm::vec3 get_direction() const { return direction; }

    /**
     * @brief Compute a point along the ray at parameter t.
     *
     * This function evaluates the parametric ray equation:
     *
     *      P(t) = origin + t * direction
     *
     * where:
     *   - origin: the starting point of the ray
     *   - direction: the normalized direction vector of the ray
     *   - t: scalar parameter indicating distance along the ray
     *
     * @param t The distance along the ray (t >= 0 for points in front of the origin).
     * @return glm::vec3 The 3D point on the ray at distance t from the origin.
     *
     * @note This is typically used to compute intersection points once a valid t
     *       is found by solving an object’s intersection equation (e.g., sphere, plane).
     */
    glm::vec3 at(float t) const { return origin + t * direction; }
};

#endif // RAYTRACINGCPP_SRC_UTILITIES_RAY_HPP