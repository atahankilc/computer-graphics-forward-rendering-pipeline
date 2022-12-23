#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <iostream>
#include "Triangle.h"
#include "Transformation/Base.h"

using namespace std;

class Mesh
{

public:
    int meshId;
    int type; // 0 for wireframe, 1 for solid
    int numberOfTransformations;
    vector<int> transformationIds;
    vector<char> transformationTypes;
    int numberOfTriangles;
    vector<Triangle> triangles;

    vector<Base*> transformations;
    Matrix4 modelingTransform;

    Mesh();
    Mesh(int meshId, int type, int numberOfTransformations,
          vector<int> transformationIds,
          vector<char> transformationTypes,
          int numberOfTriangles,
          vector<Triangle> triangles);

    void computeModelingTransform();
};

#endif