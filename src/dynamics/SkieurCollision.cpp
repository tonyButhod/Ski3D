#include "./../../include/dynamics/SkieurCollision.hpp"
#include <glm/gtc/matrix_transform.hpp>

SkieurCollision::SkieurCollision(ParticleSkieurPtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

SkieurCollision::~SkieurCollision()
{}

void SkieurCollision::do_solveCollision()
{
    //Don't process fixed particles (ground plane is assumed fixed)
    if (m_particle->isFixed())
        return;
    glm::vec3 normal(0.0,0.0,1.0);
    if (m_particle->getJumpCollision()) {
        //On calcule les collisions avec les sauts
        JumpRenderablePtr j = m_particle->getJump();
        const glm::vec3 &pMin = j->getCornerMin();
        const glm::vec3 &pMax = j->getCornerMax();
        glm::vec3 pos = m_particle->getPosition();
        if (pos[0] > pMin[0] && pos[0] < pMax[0] && pos[1] > pMin[1] && pos[1] < pMax[1]) {
            float coef = (pMax[1] - pos[1])/(pMax[1]-pMin[1]);
            if (pos[2]-m_particle->getRadius() <= pMin[2] + coef*(pMax[2] - pMin[2])) {
                pos[2] = pMin[2] + coef*(pMax[2] - pMin[2]) + m_particle->getRadius();
                m_particle->setPosition(pos);
            }
        }
        normal = j->getNormal();
    }
    else if (m_particle->getCollision()) {
        normal = m_plane->normal();
        float dist = std::abs(glm::dot(m_particle->getPosition(), normal)-m_plane->distanceToOrigin());
        m_particle->setPosition(m_particle->getPosition() + (m_particle->getRadius() - dist)*normal);
    }
    
    float proj_v = (1.0f + m_restitution) * glm::dot(normal, m_particle->getVelocity());
    m_particle->setVelocity(m_particle->getVelocity() - proj_v*normal);
    //Pour que le skieur glisse dans la bonne direction
    glm::vec3 rotation = m_particle->getRotation();
    glm::vec3 ortho_ski = glm::normalize(glm::vec3(-sin(rotation[2]), cos(rotation[2]), 0.0));
    float proj_ski = glm::dot(m_particle->getVelocity(), ortho_ski);
    m_particle->setVelocity(m_particle->getVelocity() - proj_ski*ortho_ski);
    //Réaction tangetielle du suppport.
    float frottement = 0.001f;
    m_particle->setVelocity((1.0f - frottement) * m_particle->getVelocity());
    //On enregistre la normale au sol lors de la collision pour orienter le skieur.
    float coef = (normal[0]<0)?-1.0f:1.0f;
    float anglez = acos(glm::dot(normal,glm::vec3(0,0,1))) - coef*rotation[1];
    m_particle->setRotation(m_particle->getRotation() + 0.1f*glm::vec3(0.0, coef*anglez, 0));
}



bool testParticleSkieurPlane(const ParticleSkieurPtr &particle, const PlanePtr &plane)
{
    particle->setCollision(false);
    particle->setJumpCollision(false);
    particle->setJump(NULL);
    //On teste les collisions avec les sauts
    for (JumpRenderablePtr j : plane->getJumps()) {
        const glm::vec3 &pMin = j->getCornerMin();
        const glm::vec3 &pMax = j->getCornerMax();
        const glm::vec3 &pos = particle->getPosition();
        if (pos[0] > pMin[0] && pos[0] < pMax[0] && pos[1] > pMin[1] && pos[1] < pMax[1]) {
            float coef = (pMax[1] - pos[1])/(pMax[1]-pMin[1]);
            if (pos[2]-particle->getRadius() <= pMin[2] + coef*(pMax[2] - pMin[2])) {
                particle->setJumpCollision(true);
                particle->setJump(j);
                return true;
            }
        }
    }
    //On teste les collisions avec le plan
    if (std::abs(glm::dot(particle->getPosition(), plane->normal())-plane->distanceToOrigin()) <= particle->getRadius()) {
        particle->setCollision(true);
        return true;
    }
    return false;
}
