//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_CAMERA_H
#define LAB4B_DEV_1_0_CAMERA_H

#define _USE_MATH_DEFINES

#include <cmath>
#include "vec3.h"

class camera {
public:
    float xmax, ymax;
    vec3 position;
    vec3 at;
    vec3 up;
    float angle;
    float fov;
    int h, w;
    float ratio;

    camera(vec3 pos, vec3 lookat, vec3 vert, float horizFov, int height, int width) {
        position = pos;
        at = lookat;
        up = vert;
        fov = horizFov;
        h = height;
        w = width;
        ratio = float(w) / float(h);
        float rad = degToRad(fov);
        angle = tan(rad);
    }

    float degToRad(float d) {
        return static_cast<float>(d * M_PI / 180);
    }

    ray getViewRay(float x, float y) {
        vec3 u, v, w, lowerleftcorner, horizontal, vertical;
        vec3 origin = position;
        vec3 direction;

        float halfW = ratio * angle;

        w = position - at;
        w.normalize();
        u = cross(up, w);
        u.normalize();
        v = cross(w, u);

        lowerleftcorner = origin - halfW * u - angle * v - w;
        horizontal = 2 * halfW * u;
        vertical = 2 * angle * v;
        direction = lowerleftcorner + x * horizontal + y * vertical - origin;
        direction.normalize();

        return {origin, direction};

    }

};

ostream &operator<<(std::ostream &os, const camera &c) {
    os << "pos: " << c.position << endl <<
       "lookat: " << c.at << endl <<
       "up: " << c.up << endl <<
       "fov: " << c.fov << endl <<
       "height: " << c.h << endl <<
       "width: " << c.w << endl <<
       "ratio: " << c.ratio << endl <<
       "ymax: " << c.ymax << endl <<
       "xmax: " << c.xmax << endl;
    return os;
}

#endif //LAB4B_DEV_1_0_CAMERA_H
