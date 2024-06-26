#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include <memory>

class material {
    public:
        virtual ~material() = default;

        virtual color emitted() const {
            return color(0, 0, 0);
        }

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

class diffuse_light : public material {
    public:
        diffuse_light(const color& a) : emit(a) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& atternuation, ray& scattered)
        const override {
            return false;
        }

        color emitted() const override {
            return emit;
        }

    private:
        color emit;
        

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
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;

            vec3 direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }
            scattered = ray(rec.p, direction);
            return true;
        }
    private:
        double ir;

        static double reflectance(double cosine, double ref_idx) {
            // Schlick's approximation for reflectance
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1-cosine), 5);
        }
};

#endif
