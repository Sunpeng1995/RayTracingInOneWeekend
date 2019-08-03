#ifndef __VEC3_H__
#define __VEC3_H__

#include <cmath>
#include <iostream>

class vec3 {
public:
    vec3() {}
    vec3(float e0, float e1, float e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vec3 &operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float &operator[](int i) { return e[i];}

    inline vec3 operator+(const vec3 &v2) const {
        return vec3(e[0] + v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]);
    }
    inline vec3 operator-(const vec3 &v2) const {
        return vec3(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]);
    }
    inline vec3 operator*(const vec3 &v2) const {
        return vec3(e[0] * v2.e[0], e[1] * v2.e[1], e[2] * v2.e[2]);
    }
    inline vec3 operator/(const vec3 &v2) const {
        return vec3(e[0] / v2.e[0], e[1] / v2.e[1], e[2] / v2.e[2]);
    }
    inline vec3 operator*(const float &t) const {
        return vec3(e[0] * t, e[1] * t, e[2] * t);
    }
    inline vec3 operator/(const float &t) const {
        return vec3(e[0] / t, e[1] / t, e[2] / t);
    }
    inline vec3 operator+=(const vec3 &v2) {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        e[2] += v2.e[2];
        return *this;
    }

    inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline float squared_length() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    inline vec3 normalize() const {
        return *this / this->length();
    }

  private:
    float e[3];
};

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.y()*v2.z() - v1.z()*v2.y(), 
                -(v1.x()*v2.z() - v1.z()*v2.x()),
                v1.x()*v2.y() - v1.y()*v2.x());
}

inline vec3 operator*(const float &t, const vec3 &v) {
    return vec3(v[0]*t, v[1]*t, v[2]*t);
}

#endif