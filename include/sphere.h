#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "hitable.h"
#include "material.h"

class sphere : public hitable {
public:
    sphere() : sphere(vec3(0,0,0), 1, new lambertian(vec3(1,1,1))) {}
    sphere(vec3 center, float radius, material *mat) : center(center), radius(radius), mat(mat) {}

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &hr) const override;

private:
    vec3 center;
    float radius;
    material* mat;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &hr) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), oc); // * 2.0
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c; // 4*a*c
    if (discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / a;
        if (t < t_max && t > t_min) {
            hr.t = t;
            hr.position = r.pointAtParameter(t);
            hr.normal = (hr.position - center) / radius;
            hr.mat_ptr = mat;
            return true;
        }
        t = (-b + sqrt(discriminant)) / a;
        if (t < t_max && t > t_min) {
            hr.t = t;
            hr.position = r.pointAtParameter(t);
            hr.normal = (hr.position - center) / radius;
            hr.mat_ptr = mat;
            return true;
        }
    }
    return false;
}

#endif