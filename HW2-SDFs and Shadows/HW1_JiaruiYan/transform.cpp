#include "transform.h"

Transform::Transform() :
    mTranslation(Vector3f(0, 0, 0)), mEulerAngle(Vector3f(0, 0, 0)), mScale(Vector3f(1, 1, 1))
{
    glm::mat4 RMat = glm::rotate(glm::mat4(), mEulerAngle.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(), mEulerAngle.y, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(), mEulerAngle.z, glm::vec3(0, 0, 1));
    mWorldTransform = glm::translate(glm::mat4(), mTranslation) * RMat * glm::scale(glm::mat4(), mScale);
    mWorldTransformInverse = glm::inverse(mWorldTransform);
    mWorldTransformInverseTranspose = glm::mat3(glm::transpose(mWorldTransformInverse));
}

Transform::Transform(glm::vec3 it, glm::vec3 ir, glm::vec3 is) :
    mTranslation(it), mEulerAngle(ir), mScale(is)
{
    glm::mat4 RMat = glm::rotate(glm::mat4(), mEulerAngle.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(), mEulerAngle.y, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(), mEulerAngle.z, glm::vec3(0, 0, 1));
    mWorldTransform = glm::translate(glm::mat4(), mTranslation) * RMat * glm::scale(glm::mat4(), mScale);
    mWorldTransformInverse = glm::inverse(mWorldTransform);
    mWorldTransformInverseTranspose = glm::mat3(glm::transpose(mWorldTransformInverse));
}
