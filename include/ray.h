#ifndef __RAY_H__
#define __RAY_H__
#include "vec3.h"

#ifdef _WIN32
double drand48() {
    return (double)rand()/RAND_MAX;
}
#endif

class ray {
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction, float time) {
        O = origin;
        D = direction;
        _time = time;
    }
    vec3 origin() const { return O; }
    vec3 direction() const { return D; }
    float time() const { return _time; }
    vec3 pointAtParameter(float t) const { return O + t * D; }

  private:
    vec3 O;
    vec3 D;
    float _time;
};

#endif