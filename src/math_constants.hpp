#ifndef MATH_CONSTANTS_HPP
#define MATH_CONSTANTS_HPP

#include <limits>

// Constants
const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility
inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

#endif
