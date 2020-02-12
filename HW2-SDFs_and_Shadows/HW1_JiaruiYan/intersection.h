#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "globalincludes.h"
class Primitive;

class Intersection
{
public:
    // The point on the surface (in world space) at which the intersection occurred.
    Point3f mPoint;
    // The surface normal at the point of intersection in world space.
    Normal3f mNormal;
    // The distance along the Ray at which this point of intersection lies.
    float t;
    // A pointer to the Primitive whose surface we are intersecting.
    const Primitive* mPtrObjectHit;

    Intersection();
private:

};

#endif // INTERSECTION_H
