#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

#include "material.hpp"
#include "hittable.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "math_constants.hpp"
#include "random.hpp"


class camera {
    public:
        int sensor_height = 720;
        int sensor_width = 1280;
        int spp = 10;
        int max_bounces = 12;
        double vfov = 90;
        point3 lookfrom = point3(0,0,-1);
        point3 lookat = point3(0,0,0);
        vec3 vup = vec3(0,1,0);

        void render(const hittable& world) {
            init();
            
            std::cout << "P3\n" << sensor_width << ' ' << sensor_height << "\n255\n";

            for(int j = 0; j < sensor_height; ++j) {
                std::clog << "\rScanlines remaining: " << (sensor_height - j) << std::flush;
                for (int i = 0; i < sensor_width; ++i) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < spp; ++sample) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_bounces, world);
                    }
                    write_color(std::cout, pixel_color, spp);
                }
            }
        std::clog << "\rDone.                 \n";
        }

    private:
        point3 origin;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w;

        void init() {
            
            origin = lookfrom;

            double focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            double aspect_ratio = static_cast<double>(sensor_width) / static_cast<double>(sensor_height);
            auto viewport_height = 2*h*focal_length;
            double viewport_width = viewport_height * aspect_ratio;

            // Calculate camera basis vectors
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Viewport edges vectors
            vec3 viewport_u = viewport_width * u; 
            vec3 viewport_v = viewport_height * (-v);

            // Consider deltas due to pixel spacing
            pixel_delta_u = viewport_u / sensor_width;
            pixel_delta_v = viewport_v / sensor_height;

            // Upper left pixel location
            point3 viewport_upper_left =
                origin
                - (focal_length * w)
                - viewport_u/2 
                - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) {
            // Get random ray for the pixel at location i,j
            
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = origin;
            auto ray_direction = pixel_sample - ray_origin;
            
            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() const {
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();

            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }

        color ray_color(const ray& r, int bounces, const hittable& world) {
            hit_record rec;

            if (bounces <= 0) {
                return color(0, 0, 0);
            }

            if (world.hit(r, interval(0.001, INF), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, bounces - 1, world);
                }
                return color(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.2, 0.2, 0.666666);
        }

};

#endif
