#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"

#include <iostream>


using color = vec3;

inline double linear_to_gamma(double linear_component) {
    return pow(linear_component, 1/2.2);
}


void write_color(std::ostream &out, color pixel_color, int spp) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / spp;
    r *= scale;
    g *= scale;
    b *= scale;

    // Gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0, 255] value  to each color component
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif
