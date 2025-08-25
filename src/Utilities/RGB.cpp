#include "RGB.hpp"
#include <algorithm>
#include <cmath>

// --- Constructors ---
RGB::RGB() : r(0), g(0), b(0) {}

RGB::RGB(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

RGB::RGB(int r_, int g_, int b_) : r(r_), g(g_), b(b_) {
    clamp();
}

// --- Clamp ---
void RGB::clamp() {
    r = std::min(255.f, std::max(0.f, r));
    g = std::min(255.f, std::max(0.f, g));
    b = std::min(255.f, std::max(0.f, b));
}

// --- Operators ---
RGB RGB::operator*(float scalar) const {
    return RGB(r * scalar, g * scalar, b * scalar);
}

RGB& RGB::operator*=(float scalar) {
    r *= scalar; g *= scalar; b *= scalar;
    clamp();
    return *this;
}

RGB RGB::operator+(const RGB& other) const {
    return RGB(r + other.r, g + other.g, b + other.b);
}

RGB& RGB::operator+=(const RGB& other) {
    r += other.r; g += other.g; b += other.b;
    clamp();
    return *this;
}

// --- Conversion to int tuple ---
std::tuple<int, int, int> RGB::to_int_tuple() const {
    return {static_cast<int>(std::round(r)),
            static_cast<int>(std::round(g)),
            static_cast<int>(std::round(b))};
}

// --- PPM output ---
void RGB::print_ppm(std::ostream& out) const {
    auto [ri, gi, bi] {to_int_tuple()};
    out << ri << " " << gi << " " << bi << "\n";
}
