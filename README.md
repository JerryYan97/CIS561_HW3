Ray Tracer Part III: Sampling Functions
======================

**University of Pennsylvania, CIS 561: Advanced Computer Graphics, Homework 3**

Overview
------------
You will implement a collection of functions in order to generate sample points
on the surfaces of objects, which will help you to implement the beginning of
your path tracer later on. Sampling the surfaces of different shapes is very
important in a path tracer; not only does one have to cast rays in random
directions within a hemisphere, but if one wants to sample rays to area
lights, one needs to sample points on the surfaces of these lights.

Once you have implemented the various sampling functions, you will apply some
of them in your ray tracer to implement  soft shadows with area lights. We have provided base code in which to test your sampling functions,
but you will ultimately continue to work with your ray tracer code as well.

Useful Reading
---------
We highly recommend you refer to the textbook when implementing this homework
assignment. Chapter 7 and Chapter 13 of PBRT in particular are quite relevant.

Square Sampling Functions (10 points)
--------
In `sampler.cpp`, you will find a function called `generateSamples`. In this
function, fill out the switch statement cases for generating grid-aligned
samples and stratified samples. Each of the samples generated should fall within
the range [0, 1) on the X and Y axes. You may refer to the method used to
generate purely random samples to see how to use the provided `rng32` random
number generator. The [PCG web site](http://www.pcg-random.org/) goes into
detail as to why the RNG32 is a superior random number generator to, say,
`std::rand()`.

Sample Warping Functions (30 points)
------
In `warpfunctions.cpp`, you will find a collection of functions that throw
runtime exceptions:
* `squareToDiskUniform`
* `squareToDiskConcentric`
* `squareToSphereUniform`
* `squareToSphereCapUniform`
* `squareToHemisphereUniform`
* `squareToHemisphereCosine`

Replace the runtime exceptions with code that takes the input square sample and
warps it to the surface of the shape indicated by the function name. For the
disk warp functions, there are two implementations. For
`squareToDiskUniform`, implement a "polar" mapping where one square axis maps
to a disc radius and the other axis maps to an angle on the disc. For
`squareToDiskConcentric`, implement [Peter Shirley's warping method](https://pdfs.semanticscholar.org/4322/6a3916a85025acbb3a58c17f6dc0756b35ac.pdf)
that better preserves relative sample distances.

Likewise, there are two implementations for hemisphere sampling. Unlike the disc
sampling functions, these methods are meant to have very different distributions
of samples. For `squareToHemisphereUniform`, you must distribute all square
samples uniformly across the hemisphere surface. For `squareToHemisphereCosine`,
you must bias the warped samples toward the pole of the hemisphere and away from
the base.

If you refer to `utils.h`, you will find some useful values defined, such as
`INV_PI`, which make your computations slightly faster.

Sample Warping Probability Density Functions (20 points)
-------------
As you implemented the warping functions above, you likely noticed additional
functions with the suffix `PDF`. You must implement these functions so that they
return the result of the probability density function associated with each
warping method, using the sample point as input to the PDF. Note that most of
the PDFs will return a constant value regardless of the input point, but some
of them _are_ dependent on it. Once you have implemented all of the sample
warping functions, you can test your PDF implementations by pressing the button
at the bottom of the GUI. Each of your PDFs should evaluate to approximately
1.0, by definition.

Sphere Cap Warping
------------
PBRT does not explicitly explain how to constrain warped samples to a subsection
of a sphere, so we'll give you a hint: consider the formula for the Z coordinate
of a point on a sphere, and consider how to shrink its possible range based on
some angle _theta_ between 0 and 180 degrees.

Example Images
-------------
Below are images of the images you should expect to generate using 1024 samples
and, unless otherwise noted, grid sampling. Some of the images have had their
camera moved for better illustration of point distribution.

Grid Sampling

![](./grid.png)

Stratified Sampling

![](./stratified.png)

Disc Warping (Uniform)

![](./discunif.png)

Disc Warping (Concentric)

![](./diskcon.png)

Sphere

![](./sphere.png)

Sphere Cap (150 degrees)

![](./spherecap.png)

Hemisphere (Uniform)

![](./hemiunif.png)

Hemisphere (Cosine Weighted)

![](./hemicos.png)

`AreaLight`s, `Disc`s, and Soft Shadows (30 points)
------------
Once you've implemented the functions to sample the surfaces of squares and
discs, you can open up your ray tracer project and add an `AreaLight` class
which inherits from your `Light` class. This class will implement its shadow
testing function a little differently. Rather than being treated as a single
point in space that can be only fully occluded or unoccluded, you will treat it
as an area in space that emits light, which can be partially occluded. Taking
N samples (where N is any large number, such as 100), test to see if a point
in the scene can see a sampled point on the surface of the light source,
changing the sampled point N times. Average the result of these samples to get
your shadow intensity.

An `AreaLight` will have to know what `Shape` it is. We only require that
you support `AreaLight`s that are `SquarePlane`s and `Disc`s.
You will also have to implement a `Disc` class, which is identical to a
`SquarePlane` but has a radius of 0.5 rather than a side length of 1.

Once you have created your implementations of these classes and functions, you
can test them by setting up a simple scene with the following parameters:
* A `SquarePlane` with 0 translation, a rotation of -90 degrees about the
X-axis, and a scale of (10, 10, 1). This will act as the floor.
* A `Sphere` that is translated by 1 in the Y-direction, with otherwise unchanged
transform data.
* An `AreaLight` with a `SquarePlane` shape translated by 3.5 in the
Y-direction, a rotation of 90 degrees about the X-axis, and a scale of (2, 2, 1).

Depending on which sampling technique you use (random vs grid vs stratified),
you will get one of these images:

Random sampling:

![](random_softshadow.png)

Grid sampling:

![](grid_softshadow.png)

Stratified sampling:

![](stratified_softshadow.png)


Code Style (10 points)
----------
For the ray tracer assignments in this course, we will be grading you on coding
style since you'll be writing nearly all of the code yourself. We will be
following the style guide defined in CIS 560; you can find it [here](https://www.cis.upenn.edu/~cis460/17fa/styleguide.html) if you need to
review it. Part of this style guide is based on the design decisions made in
`Physically Based Rendering`, so adhering to it now will help you to better
understand PBRT's code once we begin the path tracer.


Extra Credit (20 points max)
-------
If you didn't have the time to do so for the previous assignment, create a custom scene using blended SDFs and multiple light sources to render something interesting. Note that because `SmoothBlend` is a `Shape`, you can compose multiple `SmoothBlend`s together within another `SmoothBlend`. The more interesting your scene, the more points you'll score. Your scene should incorporate area lights for this assignment.

Also consider taking some of what you learned in CIS 460 to make custom `Material` classes that make use of noise functions to produce varied surface color.

Submitting your project
--------------
Along with your project code, make sure that you fill out this `README.md` file
with your name and PennKey. __Be sure to include a render of each of the three sampling methods for soft shadows, using the example scene.__ You should use at least 100 samples for your shadow calculations in these final renders.

Rather than uploading a zip file to Canvas, you will simply submit a link to
the committed version of your code you wish us to grade. If you click on the
__Commits__ tab of your repository on Github, you will be brought to a list of
commits you've made. Simply click on the one you wish for us to grade, then copy
and paste the URL of the page into the Canvas submission form.
