#ifndef SMOOTHBLEND_H
#define SMOOTHBLEND_H
#include "globalincludes.h"
#include "primitive.h"

class SmoothBlend : public Shape
{
public:
    Shape* mShape1;
    Shape* mShape2;
    float k;
    SmoothBlend();
    float sdf(Point3f p) const;
    Opt<Intersection> getIntersection(Ray iRay) const;
    Opt<Intersection> generateIntersection(Point3f p) const;
    ~SmoothBlend(){}
};

#endif // SMOOTHBLEND_H
