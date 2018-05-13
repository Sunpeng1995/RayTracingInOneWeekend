#ifndef __HITABLE_H__
#define __HITABLE_H__
#include "ray.h"

class material;

struct hit_record {
    float t;
    vec3 position;
    vec3 normal;
    material *mat_ptr;
};

class hitable {
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &hr) const = 0;
};

#endif