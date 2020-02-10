#include "smoothblend.h"

SmoothBlend::SmoothBlend()
{
    k = 0.1f;
}

Opt<Intersection> SmoothBlend::getIntersection(Ray iRay) const
{}

Opt<Intersection> SmoothBlend::generateIntersection(Point3f p) const
{
    Intersection tempIntersection;
    tempIntersection.mPoint = p;
    glm::vec3 smallXVec = glm::vec3(0.0001f, 0.f, 0.f);
    glm::vec3 smallYVec = glm::vec3(0.f, 0.0001f, 0.f);
    glm::vec3 smallZVec = glm::vec3(0.f, 0.f, 0.0001f);
    float gradientX = (sdf(p + smallXVec) - sdf(p - smallXVec));
    float gradientY = (sdf(p + smallYVec) - sdf(p - smallYVec));
    float gradientZ = (sdf(p + smallZVec) - sdf(p - smallZVec));
    tempIntersection.mNormal = glm::normalize(glm::vec3(gradientX, gradientY, gradientZ));
    return tempIntersection;
}

float SmoothBlend::sdf(Point3f p) const
{
    float a = mShape1->sdf(p);
    float b = mShape2->sdf(p);
    float h = glm::clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    float sdfLocal = glm::mix(b, a, h) - k * h * (1.0 - h);
    return sdfLocal * glm::min(this->mTransform.mScale.x, glm::min(this->mTransform.mScale.y, this->mTransform.mScale.z));
}
