#include <vector>
#include "Triangle.h"
#include "Mesh.h"
#include <iostream>
#include <iomanip>

using namespace std;

Mesh::Mesh() {}

// TODO bunu burdan al
Mesh::Mesh(int meshId, int type, int numberOfTransformations,
             vector<int> transformationIds,
             vector<char> transformationTypes,
             int numberOfTriangles,
             vector<Triangle> triangles)
{
    this->meshId = meshId;
    this->type = type;
    this->numberOfTransformations = numberOfTransformations;
    this->numberOfTriangles = numberOfTriangles;

    this->transformationIds = transformationIds;
    this->transformationTypes = transformationTypes;
    this->triangles = triangles;
}

void Mesh::computeModelingTransform() {
    Matrix4 modelingMatrix = getIdentityMatrix();
    for(auto transformation : this->transformations) {
        modelingMatrix = multiplyMatrixWithMatrix(transformation->matrix, modelingMatrix);
    }
    this->modelingTransform = modelingMatrix;
}
