#ifndef SCENE_H
#define SCENE_H

#include "qlist.h"
#include "globalincludes.h"
#include "primitive.h"
#include "camera.h"
#include "light.h"
#include <list>

class Scene
{
public:
    // QList<uPtr<Primitive>> mPrimitiveList;
    std::list<uPtr<Primitive>> mPrimitiveList;
    std::list<uPtr<Light>> mLightList;
    Camera mCamera;

    Scene();

    // Tests the input Ray for intersection with all Primitives in the Scene
    // Outputs the Intersection with the smallest non-negative t value via the Intersection* input to the function.
    // It should return false if the Ray does not intersect any geometry.
    bool getIntersection(Ray iRay, Intersection* iIntersection) const;
    bool getColor(Ray iRay, Color3f& iColor);
    Opt<Intersection> getIntersection(Ray iRay);
};

#endif // SCENE_H
