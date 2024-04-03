#include "camera.hpp"
#include "color.hpp"
#include "material.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

#include <memory>

#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <iostream>

int main() {

    // World
    hittable_list world;
    
    auto material_ground = std::make_shared<lambertian>(color(0.2, 0.8, 0.1));
    auto material_center = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_left = std::make_shared<lambertian>(color(0.2, 0.2, 0.2));
    auto material_right = std::make_shared<lambertian>(color(0.7, 0.7, 0.7));
    auto material_metal = std::make_shared<metal>(color(0.75, 0.75, 0.75), 0.05);
    auto material_metal_rough = std::make_shared<metal>(color(0.75, 0.75, 0.75), 0.7);
    auto material_glass = std::make_shared<dielectric>(1.5);

    world.add(std::make_shared<sphere>(point3( 0.0 , -100.25, -1.0), 100.0, material_glass));
    world.add(std::make_shared<sphere>(point3( 0.0 ,    0.0 , -1.0),   0.25, material_center));
    world.add(std::make_shared<sphere>(point3(-0.52,    0.0 , -1.0),   0.25, material_left));
    world.add(std::make_shared<sphere>(point3( 0.52,    0.0 , -1.0),   0.25, material_right));
    world.add(std::make_shared<sphere>(point3(-0.25,   -0.15 , -0.55),   0.10, material_metal));
    world.add(std::make_shared<sphere>(point3( 0.25,   -0.15 , -0.55),   0.10, material_metal_rough));
    world.add(std::make_shared<sphere>(point3( 0.0,    -0.15 , -0.55),   0.10, material_glass));

    // Camera
    camera cam;

    cam.sensor_width = 720;
    cam.sensor_height = 720;
    cam.vfov = 65;
    cam.lookfrom = point3(0, 0, 0);
    cam.lookat = point3(0,0,-1);
    cam.vup = vec3(0,1,0);
    cam.spp = 20;
    cam.max_bounces = 12;
    cam.defocus_angle = 1.0;
    cam.focus_dist = 1.0;
  
    // Rendering
    cam.render(world);

    return 0;
}
