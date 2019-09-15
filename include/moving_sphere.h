#pragma once
#include "hitable.h"
#include "material.h"

class moving_sphere : public hitable {
public:
    moving_sphere() {}
    moving_sphere(vec3 center0, vec3 center1, float time0, float time1, float radius, material *mat) : 
        center0(center0), center1(center1), time0(time0), time1(time1), radius(radius), mat(mat) {}

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &hr) const override;
    virtual bool bounding_box(float t0, float t1, aabb &box) const override;
    vec3 center(float time) const;

private:
    vec3 center0, center1;
    float time0, time1;
    float radius;
    material* mat;
};

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &hr) const {
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), oc); // * 2.0
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c; // 4*a*c
    if (discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / a;
        if (t < t_max && t > t_min) {
            hr.t = t;
            hr.position = r.pointAtParameter(t);
            hr.normal = (hr.position - center(r.time())) / radius;
            hr.mat_ptr = mat;
            return true;
        }
        t = (-b + sqrt(discriminant)) / a;
        if (t < t_max && t > t_min) {
            hr.t = t;
            hr.position = r.pointAtParameter(t);
            hr.normal = (hr.position - center(r.time())) / radius;
            hr.mat_ptr = mat;
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb &box) const {
    box = aabb::surrounding_box(aabb(center(t0) - vec3(radius), center(t0) + vec3(radius)),
                                aabb(center(t1) - vec3(radius), center(t1) + vec3(radius)));
    return true;
}

vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}