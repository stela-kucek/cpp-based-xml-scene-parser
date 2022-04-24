//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_LIGHT_H
#define LAB4B_DEV_1_0_LIGHT_H

#include "vec3.h"

class light {
public:
    struct ambient {
        vec3 color;

        ambient(vec3 c) { color = c; }
    };

    struct directed {
    public:
        int type; // parallel 0, point 1
        vec3 color;
        vec3 position;

        directed(int t, vec3 c, vec3 p) {
            type = t;
            color = c;
            position = p;
          //  position.normalize();
        }
    };

};


#endif //LAB4B_DEV_1_0_LIGHT_H
