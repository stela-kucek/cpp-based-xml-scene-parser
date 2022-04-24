//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_XML_READER_H
#define LAB4B_DEV_1_0_XML_READER_H

#include "pugi_xml/pugixml.cpp"
#include "pugi_xml/pugixml.hpp"
#include "vec3.h"
#include "sphere.h"
#include "camera.h"
#include "light.h"
#include "triangle.h"
#include "obj_reader.h"
#include <stdlib.h>
#include <vector>

using namespace pugi;
using namespace std;

/*
 * Read and parse file for importing geometry from
 * the given XML scene files
 *
 * Library used: pugixml from https://pugixml.org/
 */
class xml_reader {
public:
    xml_document doc;
    xml_parse_result result;

    xml_reader(char xml[]) {
        //  result = doc.load_file("..\\scenes\\example3.xml");
        result = doc.load_file(xml);
        if (result) cout << "load successful from " << xml << endl;
        else cout << "load failed" << endl;
    }

    int getMaxBounces(){
        xml_node bounces = doc.child("scene").child("camera").child("max_bounces");
        return bounces.attribute("n").as_int();
    }

    string getFileName(){
        xml_node scene = doc.child("scene");
        string fileName = scene.attribute("output_file").as_string();
        return fileName;
    }

    vector<light::directed> getParallelOrPointLight() {
        vector<light::directed> allLights;

        xml_node lights = doc.child("scene").child("lights");
        char PARALLEL[] = "parallel_light";
        char POINT[] = "point_light";
        xml_node l;
        xml_named_node_iterator it;
        vec3 color;
        vec3 pos;
        xml_node parallel_light = lights.child(PARALLEL);

        if (parallel_light) {
            l = parallel_light;
            color = vec3(l.child("color").attribute("r").as_float(),
                         l.child("color").attribute("g").as_float(),
                         l.child("color").attribute("b").as_float());
            pos = vec3(l.child("direction").attribute("x").as_float(),
                       l.child("direction").attribute("y").as_float(),
                       l.child("direction").attribute("z").as_float());
            allLights.emplace_back(light::directed(0, color, pos));
        } else {
            for (it = lights.children(POINT).begin(); it != lights.children(POINT).end(); ++it) {
                if (*it) {
                    l = *it;
                    color = vec3(l.child("color").attribute("r").as_float(),
                                 l.child("color").attribute("g").as_float(),
                                 l.child("color").attribute("b").as_float());
                    pos = vec3(l.child("position").attribute("x").as_float(),
                               l.child("position").attribute("y").as_float(),
                               l.child("position").attribute("z").as_float());
                    allLights.emplace_back(light::directed(1, color, pos));
                }
            }
        }

        return allLights;
    }


    light::ambient getAmbientLight() {
        xml_node lights = doc.child("scene").child("lights");
        xml_node l = lights.child("ambient_light");
        vec3 lColor = vec3(l.child("color").attribute("r").as_float(),
                           l.child("color").attribute("g").as_float(),
                           l.child("color").attribute("b").as_float());
        return {lColor};
    }

    camera getCamera() {

        xml_node cam = doc.child("scene").child("camera");
        vec3 pos = vec3(cam.child("position").attribute("x").as_float(),
                        cam.child("position").attribute("y").as_float(),
                        cam.child("position").attribute("z").as_float());
        vec3 at = vec3(cam.child("lookat").attribute("x").as_float(),
                       cam.child("lookat").attribute("y").as_float(),
                       cam.child("lookat").attribute("z").as_float());
        vec3 up = vec3(cam.child("up").attribute("x").as_float(),
                       cam.child("up").attribute("y").as_float(),
                       cam.child("up").attribute("z").as_float());
        float fov = cam.child("horizontal_fov").attribute("angle").as_float();
        int w = cam.child("resolution").attribute("horizontal").as_int();
        int h = cam.child("resolution").attribute("vertical").as_int();
        return {pos, at, up, fov, w, h};

    }

    float getRand() {
        return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }

    vector<triangle *> getMeshes() {
        const char SOLID[] = "material_solid";
        const char TEX[] = "material_textured";
        xml_node meshNode = doc.child("scene").child("surfaces").child("mesh");
        //get obj data
        string meshName = meshNode.attribute("name").as_string();
        string path = "scenes/" + meshName;
        //cout << "Loading obj data from '" << path << "'" << endl;
        obj_reader reader = obj_reader(path);
        vector<triangle *> ms = reader.getMeshData();

        vec3 color, phong;
        float exp, refl, tran, refr;
        xml_node materialSolid = meshNode.child(SOLID);
        xml_node materialTextured = meshNode.child(TEX);
        xml_node temp;

        //get solid material
        if (materialSolid) {

            temp = materialSolid.child("color");
            color = vec3(temp.attribute("r").as_float(),
                         temp.attribute("g").as_float(),
                         temp.attribute("b").as_float());
            temp = temp.next_sibling();

        } else if (materialTextured) {
            temp = materialTextured.child("texture");
            color = vec3(0.3, 0.3, 0.3);//vec3(getRand(), getRand(), getRand());
            /*
             * string texName = temp.attribute("name").as_string();
             * string texPath = "../scenes/" + texName;
             * code for png input
             *
             */
            temp = temp.next_sibling();
        } else {
           // cerr << "No mesh details fetched" << endl;
        }
        phong = vec3(temp.attribute("ka").as_float(),
                     temp.attribute("kd").as_float(),
                     temp.attribute("ks").as_float());
        exp = temp.attribute("exponent").as_float();
        temp = temp.next_sibling();
        refl = temp.attribute("r").as_float();
        tran = temp.attribute("t").as_float();
        refr = temp.attribute("iof").as_float();

        for (triangle *m : ms) {
            m->mat.color = color;//vec3(getRand(), getRand(), getRand());
            m->mat.phong = phong;
            m->mat.exponent = exp;
            m->mat.refl = refl;
            m->mat.refr = refr;
            m->mat.trans = tran;
        }
        return ms;
    }

    vector<sphere *> getSpheres() {
        vector<sphere *> spheres;

        xml_node surfaces = doc.child("scene").child("surfaces");
        xml_named_node_iterator it;
        float rad, x, y, z, r, g, b, ac, dc, sc, exp;
        float reflect, transmit, refract;
        char SPHERE[] = "sphere";

        for (it = surfaces.children(SPHERE).begin(); it != surfaces.children(SPHERE).end(); ++it) {
            if (*it) {
                xml_node sphereNode = *it;
                rad = sphereNode.attribute("radius").as_float();

                x = sphereNode.child("position").attribute("x").as_float();
                y = sphereNode.child("position").attribute("y").as_float();
                z = sphereNode.child("position").attribute("z").as_float();

                r = sphereNode.child("material_solid").child("color").attribute("r").as_float();
                g = sphereNode.child("material_solid").child("color").attribute("g").as_float();
                b = sphereNode.child("material_solid").child("color").attribute("b").as_float();

                ac = sphereNode.child("material_solid").child("phong").attribute("ka").as_float();
                dc = sphereNode.child("material_solid").child("phong").attribute("kd").as_float();
                sc = sphereNode.child("material_solid").child("phong").attribute("ks").as_float();

                exp = sphereNode.child("material_solid").child("phong").attribute("exponent").as_float();

                reflect = sphereNode.child("material_solid").child("reflectance").attribute("r").as_float();
                transmit = sphereNode.child("material_solid").child("transmittance").attribute("t").as_float();
                refract = sphereNode.child("material_solid").child("refraction").attribute("iof").as_float();

                sphere *s = new sphere(vec3(x, y, z), rad, vec3(r, g, b));
                s->mat.color = vec3(r, g, b);
                s->mat.type = 0;
                s->mat.phong = vec3(ac, dc, sc);
                s->mat.exponent = exp;
                s->mat.refl = reflect;
                s->mat.trans = transmit;
                s->mat.refr = refract;

                spheres.emplace_back(s);
            }
        }

        return spheres;
    }

    vec3 backgroundColor() {
        float r = doc.child("scene").child("background_color").attribute("r").as_float();
        float g = doc.child("scene").child("background_color").attribute("g").as_float();
        float b = doc.child("scene").child("background_color").attribute("b").as_float();
        return {r, g, b};
    }
};

#endif //LAB4B_DEV_1_0_XML_READER_H
