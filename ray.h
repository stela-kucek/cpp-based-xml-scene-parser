//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_RAY_H
#define LAB4B_DEV_1_0_RAY_H

#include "vec3.h"

/*
 * Class for declaring rays, defined by their
 * origin and their direction
 *
 * Includes calculation of the intersection point position
 * w.r.t. parameter t
 *
 */
class ray {
public:
    vec3 origin;
    vec3 direction;
    ray() = default;

    ray(const vec3& o, const vec3& d){
        origin = o;
        direction = d;
    }

    vec3 pointAtParam(float t) const { return origin + t*direction; }

};


#endif //LAB4B_DEV_1_0_RAY_H
