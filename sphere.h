//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_SPHERE_H
#define LAB4B_DEV_1_0_SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "surface.h"
#include <cmath> // only used for the square root function!!

/*
 * Class containing all sphere-relevant data imported from
 * the XML scene files.
 */
class sphere : public surface {
public:
    vec3 center;
    float radius;
    vec3 color;

    sphere(vec3 c, float r, vec3 col) {
        center = c;
        radius = r;
        color = vec3(col.x(), col.y(), col.z());
        normal = vec3(0,0,0);
    }

    bool isHit(const ray &) override;

    ~sphere() = default;
};

/*
    * Determine whether the primary ray (view ray) intersects
    * the sphere,
    * if so, set data of the hit point and return true;
    * false otherwise.
    *
    * Calulations derived from
    * "Raytracing in a Weekend" by Peter Shirley, Version 1.55
    */
bool sphere::isHit(const ray &r) {
    hitPoint *point = &this->hit;
    vec3 oc = r.origin - center;
    float a = dot(r.direction, r.direction);
    float b = dot(oc, r.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        float t = (-b - sqrt(b * b - a * c)) / a;
        if (t > 0.001) {
            point->t = t;
            point->p = r.pointAtParam(point->t);
            point->normal = (point->p - center) / radius;
            return true;
        }
        t = (-b + sqrt(b * b - a * c)) / a;
        if (t > 0.001) {
            point->t = t;
            point->p = r.pointAtParam(point->t);
            point->normal = (point->p - center) / radius;
            return true;
        }
    }

    return false;
};

#endif //LAB4B_DEV_1_0_SPHERE_H
