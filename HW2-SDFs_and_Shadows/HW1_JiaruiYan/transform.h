#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "globalincludes.h"

///
/// \brief The Transform class
/// represent the sequence of transformations that have been applied to a primitive
/// transform it to its current location, size, and orientation.
///
///

class Transform
{
public:
    // A 3D vector to store its translational XYZ.
    Vector3f mTranslation;

    // A 3D vector to store its scale in the XYZ directions.
    Vector3f mScale;

    // A 3D vector to represent its Euler angles of rotation, one float for each rotational axis.
    Vector3f mEulerAngle;

    // A 4x4 matrix worldTransform
    // translate(tx,ty,tz) * rotate(rx, x_axis), * rotate(ry, y_axis) * rotate(rz, z_axis) * scale(sx, sy, sz)
    glm::mat4 mWorldTransform;

    // A 4x4 matrix worldTransformInverse
    glm::mat4 mWorldTransformInverse;

    // A 3x3 matrix worldTransformInverseTranspose.
    // After the world matrix has been truncated to a 3x3 matrix to remove its translational component.
    // be used to properly transform surface normals from local object space into world space.
    glm::mat3 mWorldTransformInverseTranspose;



    Transform();
    Transform(glm::vec3 it, glm::vec3 ir, glm::vec3 is);
};

#endif // TRANSFORM_H
