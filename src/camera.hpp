#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

#include "material.hpp"
#include "hittable.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "math_constants.hpp"
#include "random.hpp"
#include "exr.hpp"

#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <iostream>


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

        double defocus_angle = 10;
        double focus_dist = 10;

        color background;

        void render(const hittable& world) {
            init();
            
            std::cout << "P3\n" << sensor_width << ' ' << sensor_height << "\n255\n";
            Imf::Array2D<Imf::Rgba> pixels(sensor_width, sensor_height);

            for(int j = 0; j < sensor_height; ++j) {
                std::clog << "\rScanlines remaining: " << (sensor_height - j) << std::flush;
                for (int i = 0; i < sensor_width; ++i) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < spp; ++sample) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_bounces, world);
                    }
                    // write_color(std::cout, pixel_color, spp);
                    write_pixel_color(pixels, i, j, pixel_color, spp);
                }
            }
        write_exr(pixels , sensor_width, sensor_height);

        std::clog << "\rDone.                 \n";
        }

    private:
        point3 origin;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void init() {
            
            origin = lookfrom;

            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            double aspect_ratio = static_cast<double>(sensor_width) / static_cast<double>(sensor_height);
            auto viewport_height = 2*h*focus_dist;
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
                - (focus_dist * w)
                - viewport_u/2 
                - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Defocus disk basis vectors.
            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
            
        }

        ray get_ray(int i, int j) {
            // Get random ray for the pixel at location i,j
            
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = (defocus_angle <= 0) ? origin : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            
            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() const {
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();

            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return origin + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int bounces, const hittable& world) {
            hit_record rec;

            if (bounces <= 0) {
                return color(0, 0, 0);
            }

            if (!world.hit(r, interval(0.001, INF), rec)) {
                return background;
            }
        
            ray scattered;
            color attenuation;
            color color_from_emission = rec.mat->emitted();
            if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
                return color_from_emission;
            }

            color color_from_scatter = attenuation * ray_color(scattered, bounces - 1, world);

            return color_from_emission + color_from_scatter;
        }

};

#endif
