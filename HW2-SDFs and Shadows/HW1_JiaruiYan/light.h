#ifndef LIGHT_H
#define LIGHT_H
#include "globalincludes.h"
#include "intersection.h"
#include "sampler.h"
class Scene;
class Shape;
class Transform;

enum LightShape{
    SQUARE, DISC
};

class Light
{
public:
    Color3f mHue;
    Point3f mPos;
    Light(){}
    virtual float ShadowTest(Opt<Intersection>& iIntersection, Scene& iScene) = 0; // true: can see the light (no shadow);
    ~Light(){}
};

class PointLight : public Light
{
public:
    PointLight(Point3f iP, Color3f iHue);
    virtual float ShadowTest(Opt<Intersection>& iIntersection, Scene& iScene);
    ~PointLight(){}
};

class AreaLight:public Light
{
public:
    Shape* mShapePtr;
    Sampler mSampler;
    LightShape mLightShape;
    AreaLight(Shape* iShapePtr, LightShape iLightShape, Color3f iHue);
    virtual float ShadowTest(Opt<Intersection>& iIntersection, Scene& iScene);
    ~AreaLight(){}
};

#endif // LIGHT_H
