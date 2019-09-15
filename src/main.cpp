#include <iostream>
#include <cfloat>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "material.h"
#include "bvh_node.h"
#include "stb_image_write.h"

// #define DEBUG_LOG
#define PROFILE_LOG

vec3 color(const ray &r, const hitable *world, int depth) {
    hit_record rec;
    // t_min != 0, avoiding refract/reflact light hits its start point
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

std::vector<hitable*> random_scene() {
    std::vector<hitable*> hitables;
    hitables.push_back(new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5))));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) { //diffuse
                    // hitables.push_back(new sphere(center, 0.2, 
                    //     new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
                    hitables.push_back(new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0, 1, 0.2, 
                        new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
                }
                else if (choose_mat < 0.95) { //metal
                    hitables.push_back(new sphere(center, 0.2, 
                        new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5 * drand48())));
                }
                else { //glass
                    hitables.push_back(new sphere(center, 0.2, new dielectric(1.5)));
                }
            }
        }
    }

    hitables.push_back(new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5)));
    hitables.push_back(new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1))));
    hitables.push_back(new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0)));

    return hitables;
}

int main() {
    int nx = 800;
    int ny = 400;
    int ns = 100;

    // std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    auto world = random_scene();
    std::cout << "world created" << std::endl;
    auto world_bvh = new bvh_node(world.begin(), world.size(), 0.001, FLT_MAX);
    std::cout << "world bvh created" << std::endl;

    vec3 lookfrom(7,2,2);
    vec3 lookat(3,1,0);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 0.07;

    camera cam(lookfrom, lookat, vec3(0,1,0), 60, float(nx) / ny, aperture, dist_to_focus, 0, 1);

    char *data = new char[nx * ny * 3];
    int progress = 0;

#ifdef PROFILE_LOG
    clock_t begin = clock();
#endif

#ifndef _OPENMP
    std::cout << "not support openmp" << std::endl;
#endif

    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            // vec3 col(0, 0, 0);
            float c1 = 0, c2 = 0, c3 = 0;
            #pragma omp parallel for reduction(+:c1,c2,c3)
            for (int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                auto col = color(r, world_bvh, 0);
                c1 += col.r();
                c2 += col.g();
                c3 += col.b();
            }

            auto col = vec3(c1, c2, c3) / float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            data[((ny - 1 - j) * nx + i) * 3] = ir;
            data[((ny - 1 - j) * nx + i) * 3 + 1] = ig;
            data[((ny - 1 - j) * nx + i) * 3 + 2] = ib;
        }

#ifdef PROFILE_LOG
        if ((j / (ny / 100)) % 10 == 0) {
            clock_t end = clock();
            double t = double(end - begin) / CLOCKS_PER_SEC;
            begin = clock();
            progress++;
            std::cout << "progress: " << progress * 10 << "%... cost " << t << "s" << std::endl;
        }
#endif

    }

    int saveflag = stbi_write_bmp("result.bmp", nx, ny, 3, data);
    delete[] data;

#ifdef DEBUG_LOG
    if (saveflag == 0)
        std::cout << "save failed" << std::endl;
    else
        std::cout << "save success" << std::endl;
#endif

    return 0;
}