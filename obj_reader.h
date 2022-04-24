//
// Created by stela on 13.01.2019.
//

#ifndef LAB4B_DEV_1_0_OBJ_READER_H
#define LAB4B_DEV_1_0_OBJ_READER_H

#include <utility>
#include "OBJ-loader/OBJ_Loader.h"
#include "triangle.h"

using namespace objl;
using namespace std;

class obj_reader {
public:
    Loader loader;
    bool loaded;

    explicit obj_reader(string path) {
        loaded = loader.LoadFile(std::move(path));
        if (loaded)
            cout << "mesh file loaded successfully" << endl;
        //else
          //  cout << "error: failed to load file" << endl;

    }


    vector<triangle*> getMeshData() {
        vector<triangle*> triangles;

        for (int i = 0; i < loader.LoadedMeshes.size(); ++i) {
            vector<vertex> vs;
            Mesh curMesh = loader.LoadedMeshes[i];
            for (int j = 0; j < curMesh.Vertices.size(); j++) {
                vertex v;
                v.position = vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y,
                                  curMesh.Vertices[j].Position.Z);
                v.normal = vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y,
                                curMesh.Vertices[j].Normal.Z);
                v.textureCoordinates = vec3(curMesh.Vertices[j].TextureCoordinate.X,
                                            curMesh.Vertices[j].TextureCoordinate.Y, 1);
                vs.push_back(v);

            }
            int tcount = 1;
            for (int j = 0; j < curMesh.Indices.size(); j += 3) {

                //cout << "TRIANGLE " << tcount << ": " << endl;
                vertex v0 = vs.at(curMesh.Indices[j]);
                //cout << v0.position << endl;
                vertex v1 = vs.at(curMesh.Indices[j + 1]);
                //cout << v1.position << endl;
                vertex v2 = vs.at(curMesh.Indices[j + 2]);
                //cout << v2.position << endl;
                triangle * t = new triangle(v0, v1, v2);

                t->normal = v0.normal;
                ++tcount;
                triangles.push_back(t);
            }

        }

        return triangles;
    }

};


#endif //LAB4B_DEV_1_0_OBJ_READER_H
