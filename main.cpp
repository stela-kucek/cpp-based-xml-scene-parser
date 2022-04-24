#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include "vec3.h"
#include "xml_reader.h"
#include "sphere.h"
#include "camera.h"
#include "triangle.h"
#include "surface.h"

using namespace std;

int MAX_BOUNCES;
vec3 backgroundColor;

/*
 * Clip color components to valid range for image output
 */
vec3 clip(vec3 color) {
    if (color.x() > 1.0) color = vec3(1.0, color.y(), color.z());
    if (color.y() > 1.0) color = vec3(color.x(), 1.0, color.z());
    if (color.z() > 1.0) color = vec3(color.x(), color.y(), 1.0);
    return color;
}

/*
 * Determine color for the pixel
 */
vec3 shade(list &objects, vector<light::directed> lights, ray viewRay, const int &depth) {
    hitPoint point;
    material material;
    vec3 color;

    surface *s = objects.getClosest(viewRay);

    if (s != nullptr) {
        point = s->hit;
        material = s->mat;
        vec3 p = point.p; // position of hit point
        vec3 n = point.normal; // normal of the hit point

        // Get material data:
        vec3 sColor = material.color;
        float aC = material.phong.x();
        float dC = material.phong.y();
        float sC = material.phong.z();
        float exp = material.exponent;

        color = backgroundColor;

        bool inside = false;
        if (dot(viewRay.direction, n) > 0) n = -n, inside = true;
        if (depth < MAX_BOUNCES) {
            if ((material.trans > 0 || material.refl > 0)) {
                float bias = 1e-4;
                vec3 reflDir = reflect(viewRay.direction, n);
                reflDir.normalize();
                ray reflectedRay(p + n * bias, reflDir);
                vec3 reflection = shade(objects, lights, reflectedRay, depth + 1);
                vec3 refraction(0, 0, 0);
                if (material.trans > 0) {
                    float ior = material.refr, eta = (inside) ? ior : 1 / ior;
                    float cosi = dot(-n, viewRay.direction);
                    float k = 1 - eta * eta * (1 - cosi * cosi);
                    vec3 refrDir = viewRay.direction * eta + n * (eta * cosi - sqrt(k));
                    refrDir.normalize();
                    ray refractedRay(p - n * bias, refrDir);
                    refraction = shade(objects, lights, refractedRay, depth + 1);
                }
                color = (reflection * material.refl + (refraction + aC * sColor) * material.trans);
            } else {
				if(!lights.size()) color = aC * sColor;
				float lambertian, specular;
                for (light::directed light : lights) {
                    vec3 lColor = light.color;
                    vec3 l; // light vector
                    if (light.type == 0) {
                        l = -light.position;
                    } else {
                        l = light.position - p; // point light
                    }
                    l.normalize();
                    ray rayToLight = ray(p, l);
                    if (!objects.isInShadow(rayToLight, s)) {
                        lambertian = fmax(dot(n, l), 0.0);
                        specular = 0.0;
                        if (lambertian > 0.0) {
                            vec3 r = reflect(-l, n);
                            vec3 v = -viewRay.direction;
                            float specularAngle = fmax(dot(r, v), 0.0);
                            specular = pow(specularAngle, exp);
                        }

                        color += vec3(aC * sColor +
                                      dC * lambertian * sColor +
                                      sC * specular * lColor);
                    }
                }
            }
        }
        objects.closest = nullptr;

    }

    color = clip(color);
    return color;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <XML_FILE_PATH>" << endl;
        return 1;
    }

    xml_reader r = xml_reader(argv[1]);
    string outputFile = r.getFileName();
    MAX_BOUNCES = r.getMaxBounces();
    backgroundColor = r.backgroundColor();
    vector<sphere *> spheres = r.getSpheres();
    vector<triangle *> triangles = r.getMeshes();
    vector<surface *> surfaces;
    surfaces.reserve(triangles.size() + spheres.size());

    for (triangle *t: triangles) {
        surfaces.push_back(t);
    }
    for (sphere *s: spheres) {
        surfaces.push_back(s);
    }


    list *objects = new list(surfaces);

    camera c = r.getCamera();
    vec3 origin = c.position; // origin of the primary ray (view ray)
    light::ambient lAmbient = r.getAmbientLight();
    vector<light::directed> lDirected = r.getParallelOrPointLight();


    int width = c.w;
    int height = c.h;

    ofstream image;
    image.open("output/" + outputFile + ".ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    /*
     * Calculation of x and y components of the primary ray partly derived from:
     * https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing
     */
    for (int y = height; y > 0; --y) {
        for (int x = 0; x < width; ++x) {
            vec3 color;
            float xRay = float(x + 0.5) / float(width);
            float yRay = float(y + 0.5) / float(height);
            ray primRay = c.getViewRay(xRay, yRay);
            color = shade(*objects, lDirected, primRay, 0);
            color = vec3(int(color.x() * 255), int(color.y() * 255), int(color.z() * 255));
            image << color.x() << " " << color.y() << " " << color.z() << "\n";
        }
    }

    return 0;
}