#include "geometry.h"
#include <iostream>
#include <float.h>
#include <cmath>

Sphere::Sphere()
{
    mOrigin = Point3f(0, 0, 0);
    mRadius = 0.5f;
}

SquarePlane::SquarePlane()
{
    mOrigin = Point3f(0, 0, 0);
    mSideLength = 1.f;
    mNormal = Vector3f(0, 0, 1.f);
}

Box::Box()
{
    b = glm::vec3(0.5, 0.5, 0.5);
}

Capsule::Capsule()
{
    a = glm::vec3(0.0, 0.5, 0.0);
    b = glm::vec3(0.0, -0.5, 0.0);
    r = 0.2;
}

Disc::Disc()
{
    mOrigin = Point3f(0.f, 0.f, 0.f);
    mNormal = Vector3f(0.f, 0.f, 1.f);
    mRadius = 0.5f;
}

Opt<Intersection> Sphere::getIntersection(Ray iRay) const
{
    Ray localRay = iRay.Transform(mTransform.mWorldTransformInverse);
    Point3f localPos = localRay.pos;
    Vector3f localDir = localRay.dir;

    glm::vec3 oc = localPos - mOrigin;
    float a = glm::dot(localDir, localDir);
    float b = 2.0 * glm::dot(localDir, oc);
    float c = glm::dot(oc, oc) - mRadius * mRadius;

    float discriminant = b * b - 4 * a * c;
    if(discriminant > 0)
    {
        Intersection tempIntersection;
        float temp_t = (-b - glm::sqrt(discriminant)) / (2.f * a);
        if(temp_t < 0.f)
        {
            temp_t = (-b + glm::sqrt(discriminant)) / (2.f * a);
        }

        if(temp_t < 0.f)
        {
            return std::experimental::nullopt;
        }
        else
        {
            tempIntersection.t = temp_t;
            tempIntersection.mPoint = localDir * temp_t + localPos;
            tempIntersection.mNormal =
                    glm::normalize(mTransform.mWorldTransformInverseTranspose * tempIntersection.mPoint);
            return Opt<Intersection>(tempIntersection);
        }
    }
    else
    {
        return std::experimental::nullopt;
    }
}

Opt<Intersection> SquarePlane::getIntersection(Ray iRay) const
{
    Ray localRay = iRay.Transform(mTransform.mWorldTransformInverse);
    Point3f localPos = localRay.pos;
    Vector3f localDir = localRay.dir;

    float t = glm::dot(mNormal, (mOrigin - localPos)) / glm::dot(mNormal, localDir);

    Point3f intersectionPoint = localDir * t + localPos;
    Normal3f worldNormal = Normal3f(mTransform.mWorldTransformInverseTranspose * mNormal);


    if(intersectionPoint.x < 0.5 && intersectionPoint.x > -0.5 && intersectionPoint.y < 0.5 && intersectionPoint.y > -0.5)
    {
        Intersection tempIntersection;
        tempIntersection.mPoint = Point3f(mTransform.mWorldTransform * glm::vec4(intersectionPoint, 1.0f));
        tempIntersection.mNormal = worldNormal;
        tempIntersection.t = t;
        return Opt<Intersection>(tempIntersection);
    }
    else
    {
        return std::experimental::nullopt;
    }
}

Opt<Intersection> Box::getIntersection(Ray) const
{}

Opt<Intersection> Capsule::getIntersection(Ray) const
{}

Opt<Intersection> Disc::getIntersection(Ray) const
{}

float Sphere::sdf(Point3f p) const
{
    glm::vec4 homoP = glm::vec4(p, 1.0f);
    glm::vec4 localP = mTransform.mWorldTransformInverse * homoP;
    p = glm::vec3(localP.x, localP.y, localP.z);
    // return (glm::length(p - mOrigin) - mRadius);
    float sdfLocal = (glm::length(p - mOrigin) - mRadius);
    return sdfLocal * glm::min(this->mTransform.mScale.x, glm::min(this->mTransform.mScale.y, this->mTransform.mScale.z));
}

float dot2(glm::vec3 v)
{
    return glm::dot(v, v);
}

float SquarePlane::sdf(Point3f p) const
{
    glm::vec4 homoP = glm::vec4(p, 1.0f);
    glm::vec4 localP = mTransform.mWorldTransformInverse * homoP;
    p = glm::vec3(localP.x, localP.y, localP.z);
    Point3f a = Point3f(-0.5, 0.5, 0);
    Point3f b = Point3f(0.5, 0.5, 0);
    Point3f c = Point3f(0.5, -0.5, 0);
    Point3f d = Point3f(-0.5, -0.5, 0);
    Vector3f ba = b - a;
    Vector3f pa = p - a;
    Vector3f cb = c - b;
    Vector3f pb = p - b;
    Vector3f dc = d - c;
    Vector3f pc = p - c;
    Vector3f ad = a - d;
    Vector3f pd = p - d;
    Vector3f nor = glm::cross(ba, ad);

    return glm::sqrt(
        (glm::sign(glm::dot(glm::cross(ba,nor),pa)) +
         glm::sign(glm::dot(glm::cross(cb,nor),pb)) +
         glm::sign(glm::dot(glm::cross(dc,nor),pc)) +
         glm::sign(glm::dot(glm::cross(ad,nor),pd))<3.0)
         ?
         glm::min( glm::min( glm::min(
         dot2(ba * glm::clamp( glm::dot(ba,pa)/dot2(ba),0.0f,1.0f)-pa),
         dot2(cb * glm::clamp( glm::dot(cb,pb)/dot2(cb),0.0f,1.0f)-pb) ),
         dot2(dc * glm::clamp( glm::dot(dc,pc)/dot2(dc),0.0f,1.0f)-pc) ),
         dot2(ad * glm::clamp( glm::dot(ad,pd)/dot2(ad),0.0f,1.0f)-pd) )
         :
         glm::dot(nor,pa) * glm::dot(nor,pa)/dot2(nor) );
    // glm::vec4 homoP = glm::vec4(p, 1.0f);
    // glm::vec4 localP = mTransform.mWorldTransformInverse * homoP;
    // p = glm::vec3(localP.x, localP.y, localP.z);
    // return (glm::dot(p, glm::normalize(mNormal)) + 0.0f);
}

glm::vec3 mMax(glm::vec3 a, glm::vec3 b)
{
    glm::vec3 res;
    res.x = glm::max(a.x, b.x);
    res.y = glm::max(a.y, b.y);
    res.z = glm::max(a.z, b.z);
    return res;
}

glm::vec3 mMax(glm::vec3 a, float b)
{
    glm::vec3 res;
    res.x = glm::max(a.x, b);
    res.y = glm::max(a.y, b);
    res.z = glm::max(a.z, b);
    return res;
}

glm::vec3 mMin(glm::vec3 a, glm::vec3 b)
{
    glm::vec3 res;
    res.x = glm::min(a.x, b.x);
    res.y = glm::min(a.y, b.y);
    res.z = glm::min(a.z, b.z);
    return res;
}

glm::vec3 mMin(glm::vec3 a, float b)
{
    glm::vec3 res;
    res.x = glm::min(a.x, b);
    res.y = glm::min(a.y, b);
    res.z = glm::min(a.z, b);
    return res;
}

float Box::sdf(Point3f p) const
{
    glm::vec4 homoP = glm::vec4(p, 1.0f);
    glm::vec4 localP = mTransform.mWorldTransformInverse * homoP;
    p = glm::vec3(localP.x, localP.y, localP.z);

    glm::vec3 q = glm::abs(p) - b;
    // return glm::length(q) + fmin(fmax(q.x, fmax(q.y, q.z)), 0.0f);
    float sdfLocal = glm::length(mMax(q, 0.0)) + glm::min(glm::max(glm::max(q.y,q.z), q.x), 0.f);
    return sdfLocal * glm::min(this->mTransform.mScale.x, glm::min(this->mTransform.mScale.y, this->mTransform.mScale.z));
}

float Capsule::sdf(Point3f p) const
{
    glm::vec4 homoP = glm::vec4(p, 1.0f);
    glm::vec4 localP = mTransform.mWorldTransformInverse * homoP;
    p = glm::vec3(localP.x, localP.y, localP.z);

    glm::vec3 pa = p - a, ba = b - a;
    float h = glm::clamp(glm::dot(pa, ba) / glm::dot(ba, ba), 0.f, 1.f);
    // return glm::length(pa - ba * h) - r;
    float sdfLocal = glm::length(pa - ba * h) - r;
    return sdfLocal * glm::min(this->mTransform.mScale.x, glm::min(this->mTransform.mScale.y, this->mTransform.mScale.z));
}

float Disc::sdf(Point3f p) const
{
    glm::vec4 homoP = glm::vec4(p, 1.0f);
    glm::vec4 localP = mTransform.mWorldTransformInverse * homoP;
    p = glm::vec3(localP.x, localP.y, localP.z);
    float sdfLocal;

    // tempDot represents the length from current point to the disc plane.
    // float tempDot = fabs(glm::dot(p, mNormal));
    if(fabs(p.z) < 0.000001)
    {
        // In the plane of this disc.
        float tempLen = glm::length(p);
        if(tempLen <= mRadius)
        {
            sdfLocal = 0.f;
        }
        else
        {
            sdfLocal = tempLen - mRadius;
        }
    }
    else
    {
        // Out of the plane of this disc.
        Point3f castPoint = Point3f(p.x, p.y, 0.f);
        float castLen = glm::length(castPoint);
        if(castLen < mRadius)
        {
            sdfLocal = fabs(p.z);
        }
        else
        {
            float l1 = castLen - mRadius;
            sdfLocal = glm::sqrt(l1 * l1 + fabs(p.z) * fabs(p.z));
        }
    }

    return sdfLocal * glm::min(this->mTransform.mScale.x, glm::min(this->mTransform.mScale.y, this->mTransform.mScale.z));
}

Opt<Intersection> Sphere::generateIntersection(Point3f p) const
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

Opt<Intersection> SquarePlane::generateIntersection(Point3f p) const
{
    Intersection tempIntersection;
    tempIntersection.mPoint = p;
    tempIntersection.mNormal = glm::normalize(mTransform.mWorldTransformInverseTranspose * mNormal);
    return tempIntersection;
}

Opt<Intersection> Box::generateIntersection(Point3f p) const
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

Opt<Intersection> Capsule::generateIntersection(Point3f p) const
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

Opt<Intersection> Disc::generateIntersection(Point3f p) const
{
    Intersection tempIntersection;
    tempIntersection.mPoint = p;
    tempIntersection.mNormal = glm::normalize(mTransform.mWorldTransformInverseTranspose * mNormal);
    return tempIntersection;
}
