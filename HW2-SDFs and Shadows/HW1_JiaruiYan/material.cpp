#include "material.h"
#include "scene.h"
#include <cmath>

LambertMaterial::LambertMaterial(Color3f iBaseColor)
{
    mBaseColor = iBaseColor;
}

BlinnPhongMaterial::BlinnPhongMaterial(Color3f iBaseColor, float iSpec)
{
    mBaseColor = iBaseColor;
    mSpec = iSpec;
}

LightMaterial::LightMaterial(Color3f iBaseColor)
{
    mBaseColor = iBaseColor;
}

// iDir: from intersection point to view point.
Color3f LambertMaterial::GetScatteredColor(Opt<Intersection>& iIntersection, Vector3f iDir, Scene &iScene)
{
    Color3f tempColor = Color3f(0, 0, 0);
    for(auto itr = iScene.mLightList.begin(); itr != iScene.mLightList.end(); itr++)
    {
        Light* tempLightPtr = itr->get();
        Vector3f lDir = glm::normalize(tempLightPtr->mPos - iIntersection->mPoint);
        float shadowIntensity = tempLightPtr->ShadowTest(iIntersection, iScene);
        // if(tempLightPtr->ShadowTest(iIntersection, iScene))
        // {
        tempColor += (1.f - shadowIntensity) *
                     glm::clamp((glm::dot(lDir, glm::normalize(iIntersection->mNormal)) *
                                 tempLightPtr->mHue) * mBaseColor, 0.f, 255.f);
        // }
    }

    tempColor.x = glm::clamp(tempColor.x, 0.f, 255.f);
    tempColor.y = glm::clamp(tempColor.y, 0.f, 255.f);
    tempColor.z = glm::clamp(tempColor.z, 0.f, 255.f);
    //tempColor = glm::vec3(0, 255, 0);
    return tempColor;
}

Color3f BlinnPhongMaterial::GetScatteredColor(Opt<Intersection>& iIntersection, Vector3f iDir, Scene &iScene)
{
    Color3f tempColor = Color3f(0, 0, 255.f);
    iDir = -iDir;
    for(auto itr = iScene.mLightList.begin(); itr != iScene.mLightList.end(); itr++)
    {
        Light* tempLightPtr = itr->get();
        Vector3f lDir = glm::normalize(tempLightPtr->mPos - iIntersection->mPoint);
        float shadowIntensity = tempLightPtr->ShadowTest(iIntersection, iScene);
        Vector3f rDir = glm::normalize(glm::reflect(-lDir, iIntersection->mNormal));
        tempColor += (1.f - shadowIntensity) *
                     glm::clamp((glm::pow(glm::dot(iDir, rDir), mSpec) *
                     mBaseColor *
                     tempLightPtr->mHue), 0.f, 255.f);
    }
    tempColor.x = glm::clamp(tempColor.x, 0.f, 255.f);
    tempColor.y = glm::clamp(tempColor.y, 0.f, 255.f);
    tempColor.z = glm::clamp(tempColor.z, 0.f, 255.f);
    return tempColor;
}

Color3f LightMaterial::GetScatteredColor(std::experimental::optional<Intersection> &iIntersection, Vector3f iDir, Scene &iScene)
{
    return mBaseColor * 255.f;
}
