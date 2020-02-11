#include "light.h"
#include "scene.h"
#include "geometry.h"
#include "sampler.h"

PointLight::PointLight(Point3f iP, Color3f iHue)
{
    mHue = iHue;
    mPos = iP;
}

AreaLight::AreaLight(Shape* iShapePtr, LightShape iLightShape, Color3f iHue)
{
    mHue = iHue;
    mShapePtr = iShapePtr;
    mPos = iShapePtr->mTransform.mTranslation;
    mLightShape = iLightShape;
    mSampler = Sampler();
}

float PointLight::ShadowTest(Opt<Intersection> &iIntersection, Scene &iScene)
{
    Vector3f dir = glm::normalize(iIntersection->mPoint - mPos);
    Ray tempRay = Ray(mPos, dir);
    Opt<Intersection> tempIntersection = iScene.getIntersection(tempRay);
    if(tempIntersection == std::experimental::nullopt)
    {
        return 1.f;
    }
    else
    {
        if(glm::length(iIntersection->mPoint - tempIntersection->mPoint) < 0.0001)
        {
            return 0.f;
        }
        else
        {
            return 1.f;
        }
    }
}

float AreaLight::ShadowTest(std::experimental::optional<Intersection> &iIntersection, Scene &iScene)
{
    float shadowIntensity = 0.0f;
    if(mLightShape == LightShape::SQUARE)
    {
        mSampler.generateSampler(100, STRATIFIED, NONE);
    }
    else
    {
        mSampler.generateSampler(100, STRATIFIED, DISC_UNIFORM);
    }

    for(int i = 0; i < mSampler.samples.size(); i++)
    {
        // std::cout << mSampler.samples[i].x << " " << mSampler.samples[i].y << " " << mSampler.samples[i].z << std::endl;
        glm::vec4 homoLocalSamplePoint = glm::vec4(mSampler.samples[i], 1.f);
        glm::vec4 homoWorldSamplePoint = this->mShapePtr->mTransform.mWorldTransform * homoLocalSamplePoint;
        glm::vec3 mSampleWorldPos = glm::vec3(homoWorldSamplePoint);
        Vector3f dir = glm::normalize(iIntersection->mPoint - mSampleWorldPos);
        Ray tempRay = Ray(mSampleWorldPos + dir * 0.0001f, dir);
        Opt<Intersection> tempIntersection = iScene.getIntersection(tempRay);
        if(tempIntersection == std::experimental::nullopt)
        {
            shadowIntensity += 1.f;
        }
        else
        {
            if(glm::length(iIntersection->mPoint - tempIntersection->mPoint) < 0.0001)
            {
                shadowIntensity += 0.f;
            }
            else
            {
                shadowIntensity += 1.f;
            }
        }
    }
    shadowIntensity /= 100.f;
    return shadowIntensity;
}
