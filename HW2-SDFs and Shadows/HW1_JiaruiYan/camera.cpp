#include "camera.h"
#include <iostream>

Camera::Camera(unsigned int iw, unsigned int ih, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(50),
    width(iw),
    height(ih),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    worldUp(worldUp)
{
    // Assume that the reference point is our center of our screen.
    // w: from 'eye' to 'ref'.
    // u: horizontal direction for screen/image.
    // v: vertical direction for screen/image.
    Vector3f u, v, w;
    Vector3f eye2ref = ref - eye;
    float eye2refLength = glm::length(eye2ref);

    // float theta = fovy * M_PI / 180.f;
    float theta = fovy * M_PI / (180.f * 2.f);
    float aspect = width / height;

    float half_height = glm::tan(theta) * eye2refLength;
    float half_width = aspect * half_height;

    w = glm::normalize(eye2ref);
    u = glm::normalize(glm::cross(w, worldUp));
    v = glm::cross(u, w);
    lower_left_corner = ref - half_width * u - half_height * v;
    horizontal = 2.f * half_width * u;
    vertical = 2.f * half_height * v;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, worldUp);
}

glm::mat4 Camera::getInverseViewProj()
{
    return glm::inverse(getViewProj());
}

Ray Camera::rayCast(unsigned int rowIdx, unsigned int colIdx)
{
    // Input Boundary Check.
    if(rowIdx > height || colIdx > width)
    {
        std::cerr << "Camera::rayCast(float u, float v): u or v overflows." << std::endl;
        return Ray();
    }
    // Map rowIdx and colIdx to uv coordinate.
    // Here, the rowIdx and colIdx follows the QImage tradition.
    // The left-up corner is the (0, 0).
    // For uv the left-down corner is the (0, 0).
    float u = float(colIdx) / float(width);
    float v = float(height - rowIdx) / float(width);

    // uv coordinate to world coordinate.
    Point3f tar_point = lower_left_corner + u * horizontal + v * vertical;

    // create the ray.
    return Ray(eye, glm::normalize(tar_point - eye));
}
