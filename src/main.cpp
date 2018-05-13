#include <iostream>
#include <cfloat>
#include <stdlib.h>
#include <vector>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "material.h"


vec3 color(const ray &r, const hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    vec3 unit_dir = r.direction().normalize();
    float t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * vec3(1,1,1) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 800;
    int ny = 400;
    int ns = 100;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    std::vector<hitable*> list;
    list.push_back(new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.3, 0.3, 0.8))));
    list.push_back(new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0))));
    list.push_back(new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0)));
    list.push_back(new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5)));
    hitable *world = new hitable_list(list);

    camera cam;

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                col = col + color(r, world, 0);
            }
            col = col / float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}