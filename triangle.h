//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_TRIANGLE_H
#define LAB4B_DEV_1_0_TRIANGLE_H

#include "surface.h"
#include <vector>
#include "vec3.h"
#include "ray.h"


struct vertex {

    vec3 position;
    vec3 normal;
    vec3 textureCoordinates;

    /* vertex(vec3 p, vec3 n, vec3 tc) {
         position = p;
         normal = n;
         textureCoordinates = tc;
     }*/

};

class triangle : public surface {
public:

    vertex vertex0;
    vertex vertex1;
    vertex vertex2;

    triangle(vertex v0, vertex v1, vertex v2) {
        vertex0 = v0;
        vertex1 = v1;
        vertex2 = v2;
    }

    bool isHit(const ray &) override;

    ~triangle() = default;

};

bool triangle::isHit(const ray &r) {
    hitPoint *point = &this->hit;
    const float EPSILON = 0.0001;

    vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1.position - vertex0.position;
    edge2 = vertex2.position - vertex0.position;
    h = cross(r.direction, edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0 / a;
    s = r.origin - vertex0.position;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = cross(s, edge1);
    v = dot(r.direction, q) * f;
    if (v < 0.0 || u + v > 1.0)
        return false;
// At this stage we can compute t to find out where the intersection point is on the line.
    float t = dot(edge2, q) * f;
    if (t > EPSILON) // ray intersection
    {
        point->t = t;
        point->p = r.pointAtParam(point->t);
        point->normal = normal;
        return true;
    } else // This means that there is a line intersection but not a ray intersection.
        return false;
}

#endif //LAB4B_DEV_1_0_TRIANGLE_H
