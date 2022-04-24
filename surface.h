//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_SURFACE_H
#define LAB4B_DEV_1_0_SURFACE_H

#include <string>
#include "vector"
#include "vec3.h"
#include "ray.h"


/*
 * Idea for distinct struct for the intersection point
 * from "Raytracing in a Weekend" by Peter Shirley, Version 1.55
 *
 * Used as instance variable in each surface, to ease the reference
 * to the last hit point
 *
 * Includes the t parameter, the point's coordinates (position) and
 * normal of the surface at the point
 */

struct hitPoint {
    float t;
    vec3 p;
    vec3 normal;
};

struct material {
    int type; // solid 0 or textured 1
    std::string texture;
    vec3 color;
    vec3 phong;
    float exponent;
    float refl;
    float trans;
    float refr;
};

class surface {
public:
    hitPoint hit;
    material mat;
    vec3 normal;

    virtual bool isHit(const ray &) = 0;
};

class list{
public:
    vector<surface*> surfaces;
    surface* closest;

    list(vector<surface*> &v){
        surfaces = v;
        closest = nullptr;
    }

    surface* getClosest(const ray &r){
        for(surface *s: surfaces) {
            if (s->isHit(r)) {
                if (!closest) {
                    closest = s;
                } else {
                    if (s->hit.t < closest->hit.t) {
                        closest = s;
                    }
                }
            }
        }
        return closest;
    }

    bool isInShadow(const ray &r, surface * current) {

        vec3 zero(0,0,0);
        for (surface *s: surfaces){
            if (s->isHit(r) && (s != current)) {
                if(!equal(s->normal, zero) && !equal(current->normal, zero)){//its a triangle

                }
                else return true;
            }
        }
        return false;
    }
};

#endif //LAB4B_DEV_1_0_SURFACE_H
