#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Transformation/Rotation.h"
#include "Transformation/Scaling.h"
#include "Transformation/Translation.h"
#include "Triangle.h"
#include "Vector/Vec3.h"
#include "Vector/Vec4.h"

using namespace std;

class Scene {
public:
    Color backgroundColor;
    bool cullingEnabled;

    vector<vector<Color> > image;
    vector<Camera *> cameras;
    vector<Vec3 *> vertices;
    vector<Color *> colorsOfVertices;
    vector<Scaling *> scalings;
    vector<Rotation *> rotations;
    vector<Translation *> translations;
    vector<Mesh *> meshes;

    Camera *currentCamera;
    Vec4 vec1, vec2, vec3;

    Scene(const char *xmlPath);

    void initializeImage(Camera *camera);

    void forwardRenderingPipeline(Camera *camera);

    int makeBetweenZeroAnd255(double value);

    void writeImageToPPMFile(Camera *camera);

    void convertPPMToPNG(string ppmFileName, int osType);

    bool isBackFace() const;

    bool clipLine(Vec4 &v0, Vec4 &v1);

    void wireRasterization(Vec4 v0, Vec4 v1);

    void solidRasterization();
};

#endif
