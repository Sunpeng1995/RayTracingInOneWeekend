#pragma once

#include <algorithm>
#include "vec3.h"
#include "ray.h"

inline float ffmax(const float& a, const float& b) {
    return a > b ? a : b;
}
inline float ffmin(const float& a, const float& b) {
    return a < b ? a : b;
}

class aabb {
public:
    aabb() {}
    aabb(const vec3& min, const vec3& max) : _min(min), _max(max) {}

    vec3 min() const { return _min; }
    vec3 max() const { return _max; }

    bool hit(const ray& r, float tmin, float tmax) const {
        for (int i = 0; i < 3; i++) {
            float inv = 1.0f / r.direction()[i];
            float t0 = (_min[i] - r.origin()[i]) * inv;
            float t1 = (_max[i] - r.origin()[i]) * inv;
            if (inv < 0.0f)
                std::swap(t0, t1);
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin)
                return false;
        }
        return true;
    }

    static aabb surrounding_box(aabb box0, aabb box1) {
        vec3 aabbmin(fmin(box0.min().x(), box1.min().x()),
                     fmin(box0.min().y(), box1.min().y()),
                     fmin(box0.min().z(), box1.min().z()));
        vec3 aabbmax(fmax(box0.max().x(), box1.max().x()),
                     fmax(box0.max().y(), box1.max().y()),
                     fmax(box0.max().z(), box1.max().z()));

        return aabb(aabbmin, aabbmax);
    }

private:
    vec3 _min;
    vec3 _max;

};