#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "math_constants.hpp"

class interval {
    public:
        double min, max;

        interval() : min(+INF), max(-INF) {}
        
        interval(double _min, double _max) : min(_min), max(_max) {}

        bool contains(double x) const {
            return min <= x && x <= max;
        }

        bool surrounds(double x) const {
            return min < x && x < max;
        }

        double clamp(double x) const {
            if (x < min) {
                return min;
            } else if (x > max) {
                return max;
            } else {
                return x;
            }
        }

        static const interval empty, universe;
};

const static interval empty(+INF, -INF);
const static interval universe(-INF, +INF);

#endif
