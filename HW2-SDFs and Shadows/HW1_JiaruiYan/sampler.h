#ifndef SAMPLER_H
#define SAMPLER_H
#include "globalincludes.h"
#include <pcg32.h>
enum SampleMode {
    RANDOM, GRID, STRATIFIED
};

enum WarpMethod {
    NONE, DISC_UNIFORM, DISK_CONCENTRIC, SPHERE, SPHERE_CAP, HEMISPHERE_UNIFORM, HEMISPHERE_COSINE
};

class Sampler
{
private:
    //
    // uPtr<pcg32> rng1;
public:
    pcg32 rng;
    Sampler();
    void generateSampler(int numSamples, SampleMode sampleMode, WarpMethod warpMethod);
    std::vector<Point3f> samples;
};

#endif // SAMPLER_H
