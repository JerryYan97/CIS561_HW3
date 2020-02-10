#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "primitive.h"

class Sphere : public Shape
{
private:
    Point3f mOrigin;
    float mRadius;
public:
    Sphere();
    virtual Opt<Intersection> getIntersection(Ray) const;
    virtual Opt<Intersection> generateIntersection(Point3f p) const;
    virtual float sdf(Point3f p) const;
    ~Sphere(){}
};

class SquarePlane : public Shape
{
private:
    Point3f mOrigin;
    Vector3f mNormal;
    float mSideLength;
public:
    SquarePlane();
    virtual Opt<Intersection> getIntersection(Ray) const;
    virtual Opt<Intersection> generateIntersection(Point3f p) const;
    virtual float sdf(Point3f p) const;
    ~SquarePlane(){}
};

class Disc : public Shape
{
private:
    Point3f mOrigin;
    Vector3f mNormal;
    float mRadius;
public:
    Disc();
    virtual Opt<Intersection> getIntersection(Ray) const;
    virtual Opt<Intersection> generateIntersection(Point3f p) const;
    virtual float sdf(Point3f p) const;
    ~Disc(){}
};

class Box : public Shape
{
private:
    glm::vec3 b;
public:
    Box();
    virtual Opt<Intersection> getIntersection(Ray) const;
    virtual Opt<Intersection> generateIntersection(Point3f p) const;
    virtual float sdf(Point3f p) const;
    ~Box(){}
};

class Capsule : public Shape
{
private:
    glm::vec3 a;
    glm::vec3 b;
    float r;
public:
    Capsule();
    virtual Opt<Intersection> getIntersection(Ray) const;
    virtual Opt<Intersection> generateIntersection(Point3f p) const;
    virtual float sdf(Point3f p) const;
    ~Capsule(){}
};

#endif // GEOMETRY_H
