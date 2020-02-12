#include "ray.h"

Ray::Ray()
{
    pos = Point3f(0.f, 0.f, 0.f);
    dir = Vector3f(1.f, 0.f, 0.f);
}

Ray::Ray(Point3f iP, Vector3f iD)
{
    pos = iP;
    dir = iD;
}

Ray Ray::Transform(const glm::mat4 &iMat)
{
    glm::vec4 t_pos = iMat * glm::vec4(pos, 1.0);
    glm::vec4 t_dir = iMat * glm::vec4(dir, 0.0);
    Ray temp = Ray(Point3f(t_pos.x, t_pos.y, t_pos.z), Vector3f(t_dir.x, t_dir.y, t_dir.z));
    return temp;
}
