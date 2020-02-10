#include "primitive.h"


Primitive::Primitive()
{

}

Opt<Intersection> Primitive::getIntersection(Ray iRay) const
{
    Opt<Intersection> temp = mUPtrShape->getIntersection(iRay);

    if(temp != std::experimental::nullopt)
    {
        temp->mPtrObjectHit = this;
    }
    return temp;

    // The Ray does not intersect with the object.

    // Return an Intersection value if the ray intersects the object.
    // The Intersection's objectHit should point to this.
}

// The distance in primitive's local space.
// Thus, only zero value is useful.
float Primitive::sdf(Point3f p) const
{
    return mUPtrShape->sdf(p);
}

Opt<Intersection> Primitive::generateIntersection(Point3f p) const
{
    Opt<Intersection> temp = mUPtrShape->generateIntersection(p);
    if(temp != std::experimental::nullopt)
    {
        temp->mPtrObjectHit = this;
    }
    return temp;
}

Shape::Shape()
{}

Shape::Shape(Transform &iTransform)
    :   mTransform(iTransform)
{}
