#ifndef RAY_H
#define RAY_H
// #include "include/glm/glm.hpp"
#include "globalincludes.h"

class Ray
{
private:

public:
    Point3f pos;
    Vector3f dir;
    Ray();
    Ray(Point3f iP, Vector3f iD);
    Ray Transform(const glm::mat4& iMat);
};

#endif // RAY_H
