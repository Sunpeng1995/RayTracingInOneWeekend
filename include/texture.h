#pragma once
#include "vec3.h"

class texture {
public:
    virtual vec3 tex(float u, float v, const vec3& pos) const = 0;
};

class constant_texture : public texture {
public:
    constant_texture() {}
    constant_texture(vec3 c) : color(c) {}
    virtual vec3 tex(float u, float v, const vec3& pos) const {
        return color;
    }
private:
    vec3 color;
};

class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(texture *t0, texture *t1) : odd(t0), even(t1) {}
    virtual vec3 tex(float u, float v, const vec3& pos) const {
        float sines = sin(10 * pos.x()) * sin(10 * pos.y()) * sin(10 * pos.z());
        return sines < 0 ? odd->tex(u, v, pos) : even->tex(u, v, pos);
    }

private:
    texture *odd;
    texture *even;
};