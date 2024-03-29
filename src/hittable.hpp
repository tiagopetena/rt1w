#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "interval.hpp"

#include <memory>

class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        std::shared_ptr<material> mat;
        double t;
        bool is_front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // Sets the hit record normal vector.
            // Determines if the ray hits the front or back face of the normal
            // outward_normal is assumed to have unit vector

            is_front_face = dot(r.direction(), outward_normal) < 0;
            normal = is_front_face ? outward_normal : -outward_normal;   
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
