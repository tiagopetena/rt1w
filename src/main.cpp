#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

#include <memory>


int main() {

    // World
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0,0,-1), 0.25));
    world.add(std::make_shared<sphere>(point3(-101,0,-1), 100));
    world.add(std::make_shared<sphere>(point3(0,0,-101), 100));
    world.add(std::make_shared<sphere>(point3(0,102.5,-1), 100));
    world.add(std::make_shared<sphere>(point3(0,0,101.1), 100));

    // Camera
    camera cam;

    cam.sensor_width = 1280;
    cam.sensor_height = 720;
    cam.spp = 100;
    cam.max_bounces = 12;
  
    // Rendering
    cam.render(world);
}
