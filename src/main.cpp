#include <iostream>
#include "vec3.h"
#include "ray.h"

//D^2 * t + 2 * D * (O - C) * t + (O - C)^2 - R^2 = 0
bool hit_sphere(const vec3 &center, float radius, const ray &r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(r.direction(), oc);
    float c = dot(oc, oc) - radius * radius;
    return (b*b - 4*a*c > 0);
}

vec3 color(const ray& r) {
    if (hit_sphere(vec3(0, 0, -1), 0.5, r)) {
        return vec3(1, 0, 0);
    }
    vec3 unit_dir = r.direction().normalize();
    float t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * vec3(1,1,1) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 200;
    int ny = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 width(4, 0, 0);
    vec3 height(0, 2, 0);
    vec3 origin(0, 0, 0);
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / nx;
            float v = float(j) / ny;
            ray r(origin, lower_left_corner + u * width + v * height);
            vec3 col = color(r);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}