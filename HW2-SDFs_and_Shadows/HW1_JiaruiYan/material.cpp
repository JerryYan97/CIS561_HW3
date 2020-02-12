#include "material.h"
#include "scene.h"
#include <cmath>

glm::vec2 random2( glm::vec2 p ) {
    return glm::fract(glm::sin(glm::vec2(glm::dot(p, glm::vec2(127.1, 311.7)),
                 glm::dot(p, glm::vec2(269.5,183.3))))
                 * 43758.5453f);
}

float WorleyNoise(glm::vec2 uv) {
    uv *= 15.0f; // Change this to any number you want.
    glm::vec2 uvInt = glm::floor(uv);
    glm::vec2 uvFract = glm::fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            glm::vec2 neighbor = glm::vec2(float(x), float(y)); // Direction in which neighbor cell lies
            glm::vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            glm::vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbor’s Voronoi point
            float dist = glm::length(diff);
            minDist = glm::min(minDist, dist);
        }
    }
    return minDist;
}

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

RandomMaterial::RandomMaterial(Color3f iBaseColor)
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

Color3f RandomMaterial::GetScatteredColor(Opt<Intersection>& iIntersection, Vector3f iDir, Scene &iScene)
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

    float u = 0.5f + atan2(iIntersection->mNormal.z, iIntersection->mNormal.x) / (2.f * glm::pi<float>());
    float v = 0.5f - asin(iIntersection->mNormal.y) / glm::pi<float>();
    float noise = WorleyNoise(glm::vec2(u, v)); // 0.f - 1.f;
    tempColor += glm::vec3(noise * 50.f, noise * 50.f, noise * 50.f);

    tempColor.x = glm::clamp(tempColor.x, 0.f, 255.f);
    tempColor.y = glm::clamp(tempColor.y, 0.f, 255.f);
    tempColor.z = glm::clamp(tempColor.z, 0.f, 255.f);
    //tempColor = glm::vec3(0, 255, 0);
    return tempColor;
}

Color3f LightMaterial::GetScatteredColor(std::experimental::optional<Intersection> &iIntersection, Vector3f iDir, Scene &iScene)
{
    return mBaseColor * 255.f;
}
