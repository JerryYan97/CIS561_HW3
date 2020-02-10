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
        samples[i].x = sample.x;
        samples[i].y = sample.y;
        samples[i].z = 0.f;
    }
}
