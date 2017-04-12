#include "../../include/dynamics/Plane.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>


Plane::Plane(const glm::vec3& normal, const glm::vec3& point)
    : m_n{normal}, m_d{dot(normal, point)}, m_jumps(0)
{
}

Plane::Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) 
{
    m_n = glm::normalize(glm::cross(b-a, c-a));
    m_d = glm::dot(m_n, a);
    m_origin = a;
    glm::vec3 proj_n(m_n[0], m_n[1], 0.0);
    if (glm::length(proj_n) != 0) {
        proj_n = glm::normalize(proj_n);
        m_anglex = acos(glm::dot(proj_n,glm::vec3(1,0,0)));
    }
    else {
        m_anglex = 0.0f;
    }
    m_anglez = acos(glm::dot(m_n,glm::vec3(0,0,1)));
}

Plane::~Plane()
{
}

glm::vec3 Plane::projectOnPlane(const glm::vec3& p)
{
    glm::vec3 planePoint = m_d*m_n;
    glm::vec3 v = p-planePoint;
    float dist = glm::dot(v,m_n);
    glm::vec3 projection = p - dist*m_n;
    return projection;
}

void Plane::setDistanceToOrigin(const float& d)
{
    m_d = d;
}

const float& Plane::distanceToOrigin() const
{
    return m_d;
}

void Plane::setNormal(const glm::vec3& n)
{
    m_n = n;
}

const glm::vec3& Plane::normal() const
{
    return m_n;
}

void Plane::addJump(HierarchicalRenderablePtr parent, ShaderProgramPtr shader, double x, double y, double wx, double wy, double wz) {
    
    JumpRenderablePtr jump = std::make_shared<JumpRenderable>(shader);
    HierarchicalRenderable::addChild(parent, jump);
    glm::mat4 transformation = glm::mat4(1.0);
    transformation = glm::translate(transformation, m_origin);
    transformation = glm::rotate(transformation, m_anglez, glm::vec3(sin(m_anglex),cos(m_anglex),0));
    transformation = glm::translate(transformation, glm::vec3(x,y,0));
    transformation = glm::scale(transformation, glm::vec3(wx,wy,wz));
    transformation = glm::rotate(transformation, -1.570796f, glm::vec3(0,0,1));
    jump->setParentTransform(transformation);
    //On initialise les champs du saut.
    glm::vec3 pMax = glm::vec3(transformation*glm::vec4(-1, 1, 1, 1));
    glm::vec3 pMin = glm::vec3(transformation*glm::vec4(1, -1, 0, 1));
    jump->setCornerMin(pMin);
    jump->setCornerMax(pMax);
    transformation = glm::mat4(1.0);
    transformation = glm::rotate(transformation, m_anglez, glm::vec3(sin(m_anglex),cos(m_anglex),0));
    float alpha = atan(wz/(2.0*wy));
    glm::vec3 normal = glm::normalize(glm::vec3(transformation*glm::vec4(-sin(alpha), 0.0, cos(alpha), 1)));
    jump->setNormal(normal);
    m_jumps.push_back(jump);
}

const std::vector<JumpRenderablePtr> & Plane::getJumps() {
    return m_jumps;
}
