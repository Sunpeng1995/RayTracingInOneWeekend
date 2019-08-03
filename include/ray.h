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
    ray(const vec3 &origin, const vec3 &direction) {
        O = origin;
        D = direction;
    }
    vec3 origin() const { return O; }
    vec3 direction() const { return D; }
    vec3 pointAtParameter(float t) const { return O + t * D; }

  private:
    vec3 O;
    vec3 D;
};

#endif