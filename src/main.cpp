#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

#include <memory>


int main() {

    // World
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    cam.sensor_width = 1280;
    cam.sensor_height = 720;

    // Rendering
    cam.render(world);
}
