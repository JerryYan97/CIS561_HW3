#ifndef MATERIAL_H
#define MATERIAL_H
#include "globalincludes.h"
#include "intersection.h"
class Scene;

enum materialType{
    OPAQUE, TRANSPARTENT
};

class Material
{
public:
    // Represent albedo.
    Color3f mBaseColor;
    // Determine the light leaving the Material at a specified point along a specified direction.
    // Input:
    // Intersection to represent the point being shaded.
    // Vector3f to indicate the direction from which the Intersection is being viewed.
    // a reference to the Scene so that the Material can access the collection of Lights.
    virtual Color3f GetScatteredColor(Opt<Intersection>& iIntersection, Vector3f iDir, Scene& iScene) = 0;
    Material(){}
    ~Material(){}
};

class LambertMaterial : public Material
{
public:
    LambertMaterial(Color3f iBaseColor);
    ~LambertMaterial(){}
    virtual Color3f GetScatteredColor(Opt<Intersection> &iIntersection, Vector3f iDir, Scene &iScene);
};

class BlinnPhongMaterial : public Material
{
public:
    float mSpec;
    BlinnPhongMaterial(Color3f iBaseColor, float iSpec);
    ~BlinnPhongMaterial(){}
    virtual Color3f GetScatteredColor(Opt<Intersection> &iIntersection, Vector3f iDir, Scene &iScene);
};

class LightMaterial : public Material
{
public:
    LightMaterial(Color3f iBaseColor);
    ~LightMaterial(){}
    virtual Color3f GetScatteredColor(Opt<Intersection> &iIntersection, Vector3f iDir, Scene &iScene);
};

class RandomMaterial : public Material
{
public:
    RandomMaterial(Color3f iBaseColor);
    ~RandomMaterial(){}
    virtual Color3f GetScatteredColor(Opt<Intersection> &iIntersection, Vector3f iDir, Scene &iScene);
};

#endif // MATERIAL_H
