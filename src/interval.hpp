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

        static const interval empty, universe;
};

const static interval empty(+INF, -INF);
const static interval universe(-INF, +INF);

#endif
