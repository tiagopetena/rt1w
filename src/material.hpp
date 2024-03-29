#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "hittable.hpp"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            color& attenuation,
            ray& scattered
        ) const = 0;
};


class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& atternuation, ray& scattered)
        const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            atternuation = albedo;
            return true;
        }
    private:
        color albedo;
};


class metal : public material {
    public:
        metal(const color& a, double r) : albedo(a), roughness(r < 1 ? r : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + roughness * random_unit_vector());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    private:
        color albedo;
        double roughness;
};

class dielectric : public material {
    public:
        dielectric(double index) : ir(index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.is_front_face ? (1.0/ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
            
            scattered = ray(rec.p, refracted);
            return true;
        }
    private:
        double ir;
};

#endif
