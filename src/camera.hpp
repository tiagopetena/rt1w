#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

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

        void render(const hittable& world) {
            init();
            
            std::cout << "P3\n" << sensor_width << ' ' << sensor_height << "\n255\n";

            for(int j = 0; j < sensor_height; ++j) {
                std::clog << "\rScanlines remaining: " << (sensor_height - j) << std::flush;
                for (int i = 0; i < sensor_width; ++i) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < spp; ++sample) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, world);
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

        void init() {
            
            origin = point3(0, 0, 0);

            double focal_length = 2.0;
            double aspect_ratio = static_cast<double>(sensor_width) / static_cast<double>(sensor_height);

            double viewport_height = 2.0;
            double viewport_width = viewport_height * aspect_ratio;

            // Viewport edges vectors
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);

            // Consider deltas due to pixel spacing
            pixel_delta_u = viewport_u / sensor_width;
            pixel_delta_v = viewport_v / sensor_height;

            // Upper left pixel location
            point3 viewport_upper_left =
                origin
                - vec3(0, 0, focal_length)
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

        color ray_color(const ray& r, const hittable& world) {
            hit_record rec;
            if (world.hit(r, interval(0, INF), rec)) {
                vec3 direction = random_on_hemisphere(rec.normal);
                return 0.5 * ray_color(ray(rec.p, direction), world);
            }

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.2, 0.2, 0.7);
        }

};

#endif
