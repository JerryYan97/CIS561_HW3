#include "sampler.h"

Sampler::Sampler()
{
    // rng = pcg32();
    //rng1 = mkU<pcg32>(pcg32());
}

void Sampler::generateSampler(int numSamples, SampleMode sampleMode, WarpMethod warpMethod)
{
    // The square root of the number of samples input
    int sqrtVal = (int) (std::sqrt((float) numSamples) + 0.5);
    // A number useful for scaling a square of size sqrtVal x sqrtVal to 1 x 1
    float invSqrtVal = 1.f / sqrtVal;
    // pcg32 rng;
    // Ensure that the number of samples we use fits evenly within a square grid
    if(sampleMode == GRID || sampleMode == STRATIFIED)
    {
        numSamples = sqrtVal * sqrtVal;
    }

    samples.resize(numSamples);


    for(int i = 0; i < numSamples; ++i)
    {
        int y = i / sqrtVal; // The index of y.
        int x = i % sqrtVal; // The index of x.
        float xCoord = (1.f / 2.f) * invSqrtVal + x * (1.f / sqrtVal);
        float yCoord = (1.f / 2.f) * invSqrtVal + y * (1.f / sqrtVal);
        float xVibriation = (1.f / 2.f) * invSqrtVal * (rng.nextFloat() * 2.f - 1.f);
        float yVibriation = (1.f / 2.f) * invSqrtVal * (rng.nextFloat() * 2.f - 1.f);
        glm::vec2 sample;

        switch(sampleMode)
        {
        case RANDOM:
            sample = glm::vec2(rng.nextFloat(), rng.nextFloat());
            break;

        case GRID:
            {
                //TODO
                // throw std::runtime_error("You haven't yet implemented grid sampling!");
                sample = glm::vec2(xCoord, yCoord);
                break;
            }
        case STRATIFIED:
            //TODO
            // throw std::runtime_error("You haven't yet implemented stratified sampling!");
            sample = glm::vec2(xCoord + xVibriation, yCoord + yVibriation);
            break;
        }


        samples[i] = warpSample(sample, warpMethod, 150.f);

        if(warpMethod == WarpMethod::NONE)
        {
            samples[i].x = samples[i].x - 0.5f;
            samples[i].y = samples[i].y - 0.5f;
        }

        // samples[i].z = 0.f;
    }
}


// Takes the samples obtained from generateSamples and warps them to the shape provided
glm::vec3 Sampler::warpSample(glm::vec2 sample, WarpMethod warpMethod, float thetaMax)
{
    switch(warpMethod)
    {
    case NONE:
        return glm::vec3(sample, 0);
        break;

    case DISC_UNIFORM:
        return squareToDiskUniform(sample);
        break;

    case DISK_CONCENTRIC:
        // return WarpFunctions::squareToDiskConcentric(sample);
        break;

    case SPHERE:
        // return WarpFunctions::squareToSphereUniform(sample);
        break;

    case SPHERE_CAP:
        // return WarpFunctions::squareToSphereCapUniform(sample, thetaMax);
        break;

    case HEMISPHERE_UNIFORM:
        // return WarpFunctions::squareToHemisphereUniform(sample);
        break;

    case HEMISPHERE_COSINE:
        // return WarpFunctions::squareToHemisphereCosine(sample);
        break;
    }

    return glm::vec3(sample, 0);
}

glm::vec3 Sampler::squareToDiskUniform(const glm::vec2 &sample)
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
