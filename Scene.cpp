#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Transformation/Rotation.h"
#include "Transformation/Scaling.h"
#include "Transformation/Translation.h"
#include "Triangle.h"
#include "Vector/Vec3.h"
#include "tinyxml2.h"
#include "Vector/Helpers.h"

using namespace tinyxml2;
using namespace std;

bool Scene::isBackFace() const {
    Vec3 v1 = vec4to3(this->vec1);
    Vec3 v2 = vec4to3(this->vec2);
    Vec3 v3 = vec4to3(this->vec3);
    Vec3 normal = normalizeVec3(crossProductVec3(subtractVec3(v2, v1), subtractVec3(v3, v1)));
    return dotProductVec3(normal, v1) < 0;
}

static bool lineVisible(double den, double num, double &tE, double &tL) {
    double t;
    if (den > 0) {
        t = num / den;
        if (t > tL) {
            return false;
        }
        if (t > tE) {
            tE = t;
        }
    } else if (den < 0) {
        t = num / den;
        if (t < tE) {
            return false;
        }
        if (t < tL) {
            tL = t;
        }
    } else if (num > 0) {
        return false;
    }
    return true;
}

bool Scene::clipLine(Vec4 &v0, Vec4 &v1) {
    double dX = v1.x - v0.x, dY = v1.y - v0.y, dZ = v1.z - v0.z;
    Color dC = Color(v1.color.r - v0.color.r, v1.color.g - v0.color.g, v1.color.b - v0.color.b);
    double minX = 0, minY = 0, minZ = 0, maxX = this->currentCamera->horRes - 1, maxY =
            this->currentCamera->verRes - 1, maxZ = this->currentCamera->far - 1;
    double tE = 0, tL = 1;
    bool visible = false;
    if (lineVisible(dX, minX - v0.x, tE, tL) && lineVisible(-dX, v0.x - maxX, tE, tL) &&
        lineVisible(dY, minY - v0.y, tE, tL) && lineVisible(-dY, v0.y - maxY, tE, tL) &&
        lineVisible(dZ, minZ - v0.z, tE, tL) && lineVisible(-dZ, v0.z - maxZ, tE, tL)) {
        visible = true;
        if (tL < 1) {
            v1.x = v0.x + dX * tL;
            v1.y = v0.y + dY * tL;
            v1.z = v0.z + dZ * tL;
            v1.color = Color(v0.color.r + dC.r * tL, v0.color.g + dC.g * tL, v0.color.b + dC.b * tL);
        }
        if (tE > 0) {
            v0.x = v0.x + dX * tE;
            v0.y = v0.y + dY * tE;
            v0.z = v0.z + dZ * tE;
            v0.color = Color(v0.color.r + dC.r * tE, v0.color.g + dC.g * tE, v0.color.b + dC.b * tE);
        }
    }
    return visible;
}

void Scene::wireRasterization(Vec4 v0, Vec4 v1) {
    if (this->clipLine(v0, v1)) {
        double m = (v1.y - v0.y) / (v1.x - v0.x);
        if (m > 1 || m < -1) {
            double dummy0 = v0.x, dummy1 = v1.x;
            v0.x = v0.y;
            v0.y = dummy0;
            v1.x = v1.y;
            v1.y = dummy1;
        }
        if (v1.x < v0.x) {
            swap(v0, v1);
        }
        int increment = 1;
        if (v1.y < v0.y) {
            increment = -1;
        }
        double diffX = v1.x - v0.x, diffY = v0.y - v1.y;
        int y = (int) v0.y;
        int d = (int) (diffY + (increment * 0.5 * diffX));
        Color c = Color(v0.color.r,
                        v0.color.g,
                        v0.color.b);
        Color dc = multiplyColorWithScalar(subtractColor(v1.color, v0.color), 1 / diffX);
        if (m <= 1 && m >= -1) {
            for (int x = (int) v0.x; x <= (int) v1.x; x++) {
                this->image[x][y] = Color(int(c.r + 0.5),
                                          int(c.g + 0.5),
                                          int(c.b + 0.5));
                if (d * increment < 0) {
                    y += increment;
                    d += (int) (diffY + (increment * diffX));
                } else
                    d += (int) diffY;
                c = Color(c.r + dc.r,
                          c.g + dc.g,
                          c.b + dc.b);
            }
        } else {
            for (int x = (int) v0.x; x <= (int) v1.x; x++) {
                this->image[y][x] = Color(int(c.r + 0.5),
                                          int(c.g + 0.5),
                                          int(c.b + 0.5));
                if (d * increment < 0) {
                    y += increment;
                    d += (int) (diffY + (increment * diffX));
                } else
                    d += (int) diffY;
                c = Color(c.r + dc.r,
                          c.g + dc.g,
                          c.b + dc.b);
            }
        }
    }
}

static double f(double x, double y, Vec4 &a, Vec4 &b) {
    return x * (a.y - b.y) + y * (b.x - a.x) + a.x * b.y - a.y * b.x;
}

void Scene::solidRasterization() {
    int minX = min(min(this->vec1.x, this->vec2.x), this->vec3.x);
    int maxX = max(max(this->vec1.x, this->vec2.x), this->vec3.x);

    int minY = min(min(this->vec1.y, this->vec2.y), this->vec3.y);
    int maxY = max(max(this->vec1.y, this->vec2.y), this->vec3.y);

    minX = minX > 0 ? (minX < this->currentCamera->horRes ? minX : this->currentCamera->horRes - 1) : 0;
    maxX = maxX > 0 ? (maxX < this->currentCamera->horRes ? maxX : this->currentCamera->horRes - 1) : 0;
    minY = minY > 0 ? (minY < this->currentCamera->verRes ? minY : this->currentCamera->verRes - 1) : 0;
    maxY = maxY > 0 ? (maxY < this->currentCamera->verRes ? maxY : this->currentCamera->verRes - 1) : 0;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            double alpha = f(x, y, this->vec2, this->vec3) / f(this->vec1.x, this->vec1.y, this->vec2, this->vec3);
            double beta = f(x, y, this->vec3, this->vec1) / f(this->vec2.x, this->vec2.y, this->vec3, this->vec1);
            double gamma = f(x, y, this->vec1, this->vec2) / f(this->vec3.x, this->vec3.y, this->vec1, this->vec2);

            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                this->image[x][y] = Color(
                        int(alpha * this->vec1.color.r + beta * this->vec2.color.r + gamma * this->vec3.color.r + 0.5),
                        int(alpha * this->vec1.color.g + beta * this->vec2.color.g + gamma * this->vec3.color.g + 0.5),
                        int(alpha * this->vec1.color.b + beta * this->vec2.color.b + gamma * this->vec3.color.b + 0.5));
            }
        }
    }
}

void Scene::forwardRenderingPipeline(Camera *camera) {
    this->currentCamera = camera;
    Matrix4 perCamTransform = camera->perCamTransform;
    Matrix4 viewportTransform = camera->viewportTransform;

    for (auto mesh: this->meshes) {
        Matrix4 perCamModTransform = multiplyMatrixWithMatrix(perCamTransform, mesh->modelingTransform);

        for (auto triangle: mesh->triangles) {
            Vec4 perCamModVertex1 = multiplyMatrixWithVec4(perCamModTransform, Vec4(*triangle.getFirstVertex()));
            Vec4 perCamModVertex2 = multiplyMatrixWithVec4(perCamModTransform, Vec4(*triangle.getSecondVertex()));
            Vec4 perCamModVertex3 = multiplyMatrixWithVec4(perCamModTransform, Vec4(*triangle.getThirdVertex()));

            perCamModVertex1 = perCamModVertex1 / perCamModVertex1.t;
            perCamModVertex2 = perCamModVertex2 / perCamModVertex2.t;
            perCamModVertex3 = perCamModVertex3 / perCamModVertex3.t;

            this->vec1 = multiplyMatrixWithVec4(viewportTransform, perCamModVertex1);
            this->vec2 = multiplyMatrixWithVec4(viewportTransform, perCamModVertex2);
            this->vec3 = multiplyMatrixWithVec4(viewportTransform, perCamModVertex3);

            if (this->cullingEnabled && ((this->currentCamera->projectionType && isBackFace()) ||
                                         (!this->currentCamera->projectionType && !isBackFace()))) {
                continue;
            }

            switch (mesh->type) {
                case 0:
                    this->wireRasterization(vec1, vec2);
                    this->wireRasterization(vec2, vec3);
                    this->wireRasterization(vec3, vec1);
                    break;
                case 1:
                    this->solidRasterization();
                    break;
            }
        }
    }
}

Scene::Scene(const char *xmlPath) {
    const char *str;
    XMLDocument xmlDoc;
    XMLElement *pElement;

    xmlDoc.LoadFile(xmlPath);

    XMLNode *pRoot = xmlDoc.FirstChild();

    // read background color
    pElement = pRoot->FirstChildElement("BackgroundColor");
    str = pElement->GetText();
    sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

    // read culling
    pElement = pRoot->FirstChildElement("Culling");
    if (pElement != NULL) {
        str = pElement->GetText();

        if (strcmp(str, "enabled") == 0) {
            cullingEnabled = true;
        } else {
            cullingEnabled = false;
        }
    }

    // read cameras
    pElement = pRoot->FirstChildElement("Cameras");
    XMLElement *pCamera = pElement->FirstChildElement("Camera");
    XMLElement *camElement;
    while (pCamera != NULL) {
        Camera *cam = new Camera();

        pCamera->QueryIntAttribute("id", &cam->cameraId);

        // read projection type
        str = pCamera->Attribute("type");

        if (strcmp(str, "orthographic") == 0) {
            cam->projectionType = 0;
        } else {
            cam->projectionType = 1;
        }

        camElement = pCamera->FirstChildElement("Position");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

        camElement = pCamera->FirstChildElement("Gaze");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

        camElement = pCamera->FirstChildElement("Up");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

        cam->gaze = normalizeVec3(cam->gaze);
        cam->u = crossProductVec3(cam->gaze, cam->v);
        cam->u = normalizeVec3(cam->u);

        cam->w = inverseVec3(cam->gaze);
        cam->v = crossProductVec3(cam->u, cam->gaze);
        cam->v = normalizeVec3(cam->v);

        camElement = pCamera->FirstChildElement("ImagePlane");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
               &cam->left, &cam->right, &cam->bottom, &cam->top,
               &cam->near, &cam->far, &cam->horRes, &cam->verRes);

        camElement = pCamera->FirstChildElement("OutputName");
        str = camElement->GetText();
        cam->outputFileName = string(str);

        cam->computeTransforms();

        cameras.push_back(cam);

        pCamera = pCamera->NextSiblingElement("Camera");
    }

    // read vertices
    pElement = pRoot->FirstChildElement("Vertices");
    XMLElement *pVertex = pElement->FirstChildElement("Vertex");

    while (pVertex != NULL) {
        Vec3 *vertex = new Vec3();
        Color *color = new Color();

        str = pVertex->Attribute("position");
        sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

        str = pVertex->Attribute("color");
        sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

        vertex->color = *color;

        vertices.push_back(vertex);
        colorsOfVertices.push_back(color);

        pVertex = pVertex->NextSiblingElement("Vertex");
    }

    // read translations
    pElement = pRoot->FirstChildElement("Translations");
    XMLElement *pTranslation = pElement->FirstChildElement("Translation");
    while (pTranslation != NULL) {
        Translation *translation = new Translation();

        pTranslation->QueryIntAttribute("id", &translation->id);

        str = pTranslation->Attribute("value");
        sscanf(str, "%lf %lf %lf", &translation->x, &translation->y, &translation->z);

        translation->ComputeMatrix();
        translations.push_back(translation);

        pTranslation = pTranslation->NextSiblingElement("Translation");
    }

    // read scalings
    pElement = pRoot->FirstChildElement("Scalings");
    XMLElement *pScaling = pElement->FirstChildElement("Scaling");
    while (pScaling != NULL) {
        Scaling *scaling = new Scaling();

        pScaling->QueryIntAttribute("id", &scaling->id);
        str = pScaling->Attribute("value");
        sscanf(str, "%lf %lf %lf", &scaling->x, &scaling->y, &scaling->z);

        scaling->ComputeMatrix();
        scalings.push_back(scaling);

        pScaling = pScaling->NextSiblingElement("Scaling");
    }

    // read rotations
    pElement = pRoot->FirstChildElement("Rotations");
    XMLElement *pRotation = pElement->FirstChildElement("Rotation");
    while (pRotation != NULL) {
        Rotation *rotation = new Rotation();

        pRotation->QueryIntAttribute("id", &rotation->id);
        str = pRotation->Attribute("value");
        sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->x, &rotation->y, &rotation->z);

        rotation->ComputeMatrix();
        rotations.push_back(rotation);

        pRotation = pRotation->NextSiblingElement("Rotation");
    }

    // read meshes
    pElement = pRoot->FirstChildElement("Meshes");

    XMLElement *pMesh = pElement->FirstChildElement("Mesh");
    XMLElement *meshElement;
    while (pMesh != NULL) {
        Mesh *mesh = new Mesh();

        pMesh->QueryIntAttribute("id", &mesh->meshId);

        // read projection type
        str = pMesh->Attribute("type");

        if (strcmp(str, "wireframe") == 0) {
            mesh->type = 0;
        } else {
            mesh->type = 1;
        }

        // read mesh transformations
        XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
        XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

        while (pTransformation != NULL) {
            char transformationType;
            int transformationId;

            str = pTransformation->GetText();
            sscanf(str, "%c %d", &transformationType, &transformationId);

            switch (transformationType) {
                case 't':
                    mesh->transformations.push_back(this->translations[transformationId - 1]);
                    break;
                case 'r':
                    mesh->transformations.push_back(this->rotations[transformationId - 1]);
                    break;
                case 's':
                    mesh->transformations.push_back(this->scalings[transformationId - 1]);
                    break;
            }

            mesh->transformationTypes.push_back(transformationType);
            mesh->transformationIds.push_back(transformationId);

            pTransformation = pTransformation->NextSiblingElement("Transformation");
        }

        mesh->numberOfTransformations = mesh->transformationIds.size();

        // read mesh faces
        char *row;
        char *clone_str;
        int v1, v2, v3;
        XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
        clone_str = strdup(str);

        row = strtok(clone_str, "\n");
        while (row != NULL) {
            int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

            if (result != EOF) {
                mesh->triangles.push_back(
                        Triangle(this->vertices[v1 - 1], this->vertices[v2 - 1], this->vertices[v3 - 1]));
            }
            row = strtok(NULL, "\n");
        }
        mesh->numberOfTriangles = mesh->triangles.size();
        mesh->computeModelingTransform();
        meshes.push_back(mesh);

        pMesh = pMesh->NextSiblingElement("Mesh");
    }
}

void Scene::initializeImage(Camera *camera) {
    if (this->image.empty()) {
        for (int i = 0; i < camera->horRes; i++) {
            vector<Color> rowOfColors;

            for (int j = 0; j < camera->verRes; j++) {
                rowOfColors.push_back(this->backgroundColor);
            }

            this->image.push_back(rowOfColors);
        }
    } else {
        for (int i = 0; i < camera->horRes; i++) {
            for (int j = 0; j < camera->verRes; j++) {
                this->image[i][j].r = this->backgroundColor.r;
                this->image[i][j].g = this->backgroundColor.g;
                this->image[i][j].b = this->backgroundColor.b;
            }
        }
    }
}

int Scene::makeBetweenZeroAnd255(double value) {
    if (value >= 255.0)
        return 255;
    if (value <= 0.0)
        return 0;
    return (int) (value);
}

void Scene::writeImageToPPMFile(Camera *camera) {
    ofstream fout;

    fout.open(camera->outputFileName.c_str());

    fout << "P3" << endl;
    fout << "# " << camera->outputFileName << endl;
    fout << camera->horRes << " " << camera->verRes << endl;
    fout << "255" << endl;

    for (int j = camera->verRes - 1; j >= 0; j--) {
        for (int i = 0; i < camera->horRes; i++) {
            fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
        }
        fout << endl;
    }
    fout.close();
}

void Scene::convertPPMToPNG(string ppmFileName, int osType) {
    string command;

    // call command on Ubuntu
    if (osType == 1) {
        command = "convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // call command on Windows
    else if (osType == 2) {
        command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // default action - don't do conversion
    else {
    }
}
