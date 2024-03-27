#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdlib>

inline double random_double() {
    // Returns a random real double in [0, 1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real number in [min, max).
    return min + (max - min) * random_double();
}

#endif
