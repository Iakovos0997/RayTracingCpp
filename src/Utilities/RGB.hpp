#ifndef RAYTRACINGCPP_SRC_UTILITIES_RGB_HPP
#define RAYTRACINGCPP_SRC_UTILITIES_RGB_HPP
#include <tuple>
#include <iostream>

class RGB {
public:
    int r, g, b;

    // Constructors
    RGB();
    RGB(float r_, float g_, float b_);
    RGB(int r_, int g_, int b_);

    // Clamp to [0,255]
    void clamp();

    // Operators
    RGB operator*(float scalar) const;
    RGB& operator*=(float scalar);
    RGB operator+(const RGB& other) const;
    RGB& operator+=(const RGB& other);

    // Conversion to int tuple
    std::tuple<int, int, int> to_int_tuple() const;
};

#endif // RAYTRACINGCPP_SRC_UTILITIES_RGB_HPP