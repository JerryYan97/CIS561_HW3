#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>
#include "utils.h"
#include <iostream>

glm::vec3 WarpFunctions::squareToDiskUniform(const glm::vec2 &sample)
{
    //TODO
    // throw std::runtime_error("You haven't yet implemented uniform disk warping!");
    float theta = 2.f * glm::pi<float>() * sample.y;
    float r = glm::sqrt(sample.x);
    glm::vec3 resSample;
    resSample.x = glm::cos(theta) * r;
    resSample.y = glm::sin(theta) * r;
    resSample.z = 0.f;
    return resSample;
}

glm::vec3 WarpFunctions::squareToDiskConcentric(const glm::vec2 &sample)
{
    //TODO
    // throw std::runtime_error("You haven't yet implemented concentric disk warping!");
    float phi, r, u, v;
    float a = 2.f * sample.x - 1.f;
    float b = 2.f * sample.y - 1.f;
    if(a > -b)
    {
        if(a > b)
        {
            // Region 1:
            // fabs(a) > fabs(b)
            r = a;
            phi = (glm::pi<float>() / 4.f) * (b / a);
        }
        else
        {
            // Region 2:
            // fabs(b) > fabs(a)
            r = b;
            phi = (glm::pi<float>() / 4.f) * (2.f - (a / b));
        }
    }
    else
    {
        if(a < b)
        {
            // Region 3:
            r = -a;
            phi = (glm::pi<float>() / 4.f) * (4 + (b / a));
        }
        else
        {
            r = -b;
            if(b != 0)
            {
                phi = (glm::pi<float>() / 4.f) * (6 - (a / b));
            }
            else
            {
                phi = 0;
            }
        }
    }

    u = r * glm::cos(phi);
    v = r * glm::sin(phi);
    return glm::vec3(u, v, 0.f);
}

float WarpFunctions::squareToDiskPDF(const glm::vec3 &sample)
{
    //TODO
    return INV_PI;
}

glm::vec3 WarpFunctions::squareToSphereUniform(const glm::vec2 &sample)
{
    //TODO
    // throw std::runtime_error("You haven't yet implemented uniform sphere warping!");
    float z = 1.f - 2.f * sample.x;
    float x = glm::cos(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    float y = glm::sin(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    return glm::vec3(x, y, z);
}

float WarpFunctions::squareToSphereUniformPDF(const glm::vec3 &sample)
{
    //TODO
    // return 2;
    return INV_FOURPI;
}

glm::vec3 WarpFunctions::squareToSphereCapUniform(const glm::vec2 &sample, float thetaMin)
{
    //TODO
    // throw std::runtime_error("You haven't yet implemented sphere cap warping!");
    float phi = 2 * glm::pi<float>() * sample.y;
    float theta = glm::acos(sample.x) * (180.f - thetaMin) / 90.f;
    glm::vec3 resSample;
    resSample.x = glm::sin(theta) * glm::cos(phi);
    resSample.y = glm::sin(theta) * glm::sin(phi);
    resSample.z = glm::cos(theta);
    return resSample;
}

float WarpFunctions::squareToSphereCapUniformPDF(const glm::vec3 &sample, float thetaMin)
{
    //TODO
    return INV_TWOPI * (1.f / (1.f - glm::cos(glm::radians(180.f - thetaMin))));
}

glm::vec3 WarpFunctions::squareToHemisphereUniform(const glm::vec2 &sample)
{
    //TODO
    // throw std::runtime_error("You haven't yet implemented uniform hemisphere warping!");
    float z = sample.x;
    float x = glm::cos(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    float y = glm::sin(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    return glm::vec3(x, y, z);
}

float WarpFunctions::squareToHemisphereUniformPDF(const glm::vec3 &sample)
{
    //TODO
    // return 4;
    return INV_TWOPI;
}

glm::vec3 WarpFunctions::squareToHemisphereCosine(const glm::vec2 &sample)
{
    //TODO
    // throw std::runtime_error("You haven't yet implemented cosine-weighted hemisphere warping!");
    // float theta = 2.f * glm::pi<float>() * sample.y;
    // float r = glm::sqrt(sample.x);
    glm::vec3 resSample;
    resSample = squareToDiskConcentric(sample);
    resSample.z = glm::sqrt(1 - resSample.x * resSample.x - resSample.y * resSample.y);
    return resSample;
}

float WarpFunctions::squareToHemisphereCosinePDF(const glm::vec3 &sample)
{
    //TODO
    return sample.z * INV_PI;
}
