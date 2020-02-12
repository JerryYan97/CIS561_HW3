#ifndef CAMERA_H
#define CAMERA_H
#include "globalincludes.h"
#include "ray.h"
class Camera
{

    Point3f eye;
    Point3f ref;
    Vector3f worldUp;
    Point3f lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;

    float fovy;
    float near_clip;  // Near clip plane distance
    float far_clip;  // Far clip plane distance
    unsigned int width, height;  // Screen dimensions ?? in pixel number ??

public:
    Camera(){}
    Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp);
    glm::mat4 getViewProj();
    glm::mat4 getInverseViewProj();
    Ray rayCast(unsigned int rowIdx, unsigned int colIdx);
};

#endif // CAMERA_H
