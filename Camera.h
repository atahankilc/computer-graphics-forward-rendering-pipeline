#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector/Vec3.h"
#include "Vector/Helpers.h"
#include "Matrix4.h"
#include <string>

using namespace std;

class Camera
{

public:
    int cameraId;
    int projectionType; // 1 for perspective, 0 for orthographic
    Vec3 pos;
    Vec3 gaze;
    Vec3 u;
    Vec3 v;
    Vec3 w;
    double left, right, bottom, top;
    double near;
    double far;
    int horRes;
    int verRes;
    string outputFileName;

    Matrix4 cameraTransform;
    Matrix4 projectionTransform;
    Matrix4 perCamTransform;
    Matrix4 viewportTransform;

    Camera();

    Camera(int cameraId,
           int projectionType,
           Vec3 pos, Vec3 gaze,
           Vec3 u, Vec3 v, Vec3 w,
           double left, double right, double bottom, double top,
           double near, double far,
           int horRes, int verRes,
           string outputFileName);

    Camera(const Camera &other);

    void computeTransforms();
    void computeCameraTransform();
    void computeProjectionTransform();
    void computeViewportTransform();

    friend std::ostream &operator<<(std::ostream &os, const Camera &c);
};

#endif