#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

#include "hittable.hpp"
#include "color.hpp"
#include "vec3.hpp"


class camera {
    public:
        int sensor_height = 720;
        int sensor_width = 1280;
        
        void render(const hittable& world) {
            init();
            
            std::cout << "P3\n" << sensor_width << ' ' << sensor_height << "\n255\n";

            for(int j = 0; j < sensor_height; ++j) {
                std::clog << "\rScanlines remaining: " << (sensor_height - j) << "       " << std::flush;
                for (int i = 0; i < sensor_width; ++i) {
                    point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                    vec3 ray_direction = pixel_center - origin;
                    ray r(origin, ray_direction);

                    color pixel_color = ray_color(r, world);
                    write_color(std::cout, pixel_color);
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

            double focal_length = 1.0;
            double aspect_ratio = static_cast<double>(sensor_width) / static_cast<double>(sensor_height);

            double viewport_height = 2.0;
            double viewport_width = viewport_height * aspect_ratio;

            // Viewport edges vectors
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);

            // Consider deltas due to pixel spacing
            vec3 pixel_delta_u = viewport_u / sensor_width;
            vec3 pixel_delta_v = viewport_v / sensor_height;

            // Upper left pixel location
            point3 viewport_upper_left =
                origin
                - vec3(0, 0, focal_length)
                - viewport_u/2 
                - viewport_v/2;
            point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        color ray_color(const ray& r, const hittable& world) {
            hit_record rec;
            if (world.hit(r, interval(0, INF), rec)) {
                return 0.5 * (rec.normal + color(1,1,1));
            }

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.0, 0.0, 0.999999999);
        }

};


#endif
