//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_VEC3_H
#define LAB4B_DEV_1_0_VEC3_H

#include <cmath>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
 * 3D Vector class for performing vector algebra,
 * used for positions, vectors (rays) and colors
 *
 * Derived from "Raytracing in a Weekend" by Peter Shirley, Version 1.55
 */
class vec3 {
public:
    vec3() = default;

    vec3(float e0, float e1, float e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    float e[3] = {0,0,0};

    inline float x() const { return e[0]; }

    inline float y() const { return e[1]; }

    inline float z() const { return e[2]; }

    inline vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }

    inline vec3 &operator+=(const vec3 &);

    inline vec3 &operator-=(const vec3 &);

    inline vec3 &operator*=(const vec3 &);

    inline vec3 &operator/=(const vec3 &);

    inline vec3 &operator*=(const float);

    inline vec3 &operator/=(const float);


    inline float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline void normalize();
};

inline void vec3::normalize() {
    double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]};
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]};
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]};
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]};
}

inline vec3 operator*(float t, const vec3 &v) {
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline vec3 operator/(vec3 v, float t) {
    return {v.e[0] / t, v.e[1] / t, v.e[2] / t};
}

inline vec3 operator*(const vec3 &v, float t) {
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
            (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
            (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])};
}

inline bool equal(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] == v2.e[0] && v1.e[1] == v2.e[1] && v1.e[2] == v2.e[2];
}

inline vec3 &vec3::operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3 &vec3::operator*=(const vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3 &vec3::operator/=(const vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3 &vec3::operator-=(const vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3 &vec3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3 &vec3::operator/=(const float t) {
    double k = 1.0 / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline vec3 reflect(const vec3 &incident, const vec3 &normal){
    return incident - 2.f * dot(incident, normal)*normal;
}

#endif //LAB4B_DEV_1_0_VEC3_H
