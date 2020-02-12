#include "scene.h"
#include <iostream>

Scene::Scene()
{}

bool Scene::getIntersection(Ray iRay, Intersection* iIntersection) const
{
    bool flag = false;
    Opt<Intersection> nearrestIntersection;
    nearrestIntersection->t = FLT_MAX;

    // Iterater through all the primitives in the scene.
    for(auto itr = mPrimitiveList.begin(); itr != mPrimitiveList.end(); itr++)
    {
        Primitive* primPtr = itr->get();
         Opt<Intersection> tempIntersection = primPtr->getIntersection(iRay);
        //Opt<Intersection> tempIntersection = primPtr

        // Get the nearestIntersection.
        if(tempIntersection != std::experimental::nullopt)
        {
            if(tempIntersection->t < nearrestIntersection->t)
            {
                iIntersection->mNormal = tempIntersection->mNormal;
                iIntersection->mPoint = tempIntersection->mPoint;
                iIntersection->mPtrObjectHit = tempIntersection->mPtrObjectHit;
                iIntersection->t = tempIntersection->t;
                nearrestIntersection = tempIntersection;
                flag = true;
            }
        }
    }
    return flag;
}

Opt<Intersection> Scene::getIntersection(Ray iRay)
{
    Opt<Intersection> res;
    float distance = 0.f;
    Primitive* closestPrim = nullptr;

    while(distance < 50.f)
    {
        // Find the closest primitive in the scene.
        float minDist = FLT_MAX;
        for(auto itr = mPrimitiveList.begin(); itr != mPrimitiveList.end(); itr++)
        {
            Primitive* primPtr = itr->get();
            float stepDist = primPtr->sdf(iRay.pos);
            if(stepDist < minDist)
            {
                closestPrim = primPtr;
                minDist = stepDist;
            }
        }

        // Determine whether there is an intersection.
        // If there is an intersection, then break.
        // if(minDist < FLT_EPSILON)
        if(minDist < 0.00001)
        {
            break;
        }

        if(minDist < 0)
        {
            std::cout << "Error: stepDist should always be larger than 0." << std::endl;
        }

        iRay.pos += minDist * iRay.dir;
        distance += minDist;
    }

    if(distance >= 50.f)
    {
        // There is not an intersection in the scene.
        return res;
    }
    else
    {
        // There is an intersection in the scene.
        return closestPrim->generateIntersection(iRay.pos);
    }
}

bool Scene::getColor(Ray iRay, Color3f &iColor)
{
    Opt<Intersection> tempIntersection = getIntersection(iRay);
    if(tempIntersection == std::experimental::nullopt)
    {
        iColor = Color3f(0.f, 0.f, 0.f);
        return false;
    }
    else
    {
        // iColor = (tempIntersection->mNormal + glm::vec3(1, 1, 1)) * 0.5f * 255.f;
        // for(auto itr = mLightList.begin(); itr != mLightList.end(); itr++)
        // {
        //    Light* tempLightPtr = itr->get();
        iColor += tempIntersection->mPtrObjectHit->mUPtrMaterial->GetScatteredColor(tempIntersection, iRay.dir, *this);

        // }

        iColor.x = glm::clamp(iColor.x, 0.f, 255.f);
        iColor.y = glm::clamp(iColor.y, 0.f, 255.f);
        iColor.z = glm::clamp(iColor.z, 0.f, 255.f);

        return true;
    }
}
