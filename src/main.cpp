#include "math_constants.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <iostream>
#include <memory>


color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, INF, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.0, 0.0, 0.999999999);
}


int main() {

    // Image
    
    int image_width = 3840;
    int image_height = 2160;

    auto aspect_ratio = image_width / image_height;

    // World
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0,0,-0.6), 0.5));
    world.add(std::make_shared<sphere>(point3(0.3,0,-1),0.666666));
    world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/static_cast<double>(image_height));
    auto camera_center = point3(0, 0, 0);

    // Viewport edges vectors
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Consider deltas due to pixel spacing
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Upper left pixel location
    auto viewport_upper_left =
        camera_center
        - vec3(0, 0, focal_length)
        - viewport_u/2 
        - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    // Render
    
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);


        }
    }

    std::clog << "\rDone.                       \n";
}
