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
    
    float dist = std::abs(glm::dot(m_particle->getPosition(), m_plane->normal())-m_plane->distanceToOrigin());
    m_particle->setPosition(m_particle->getPosition() + (m_particle->getRadius() - dist)*m_plane->normal());
    float proj_v = (1.0f + m_restitution) * glm::dot(m_plane->normal(), m_particle->getVelocity());
    m_particle->setVelocity(m_particle->getVelocity() - proj_v*m_plane->normal());
    //Pour que le skieur glisse dans la bonne direction
    glm::vec3 rotation = m_particle->getRotation();
    glm::vec3 ortho_ski = glm::normalize(glm::vec3(-sin(rotation[2]), cos(rotation[2]), 0.0));
    float proj_ski = glm::dot(m_particle->getVelocity(), ortho_ski);
    m_particle->setVelocity(m_particle->getVelocity() - proj_ski*ortho_ski);
    //Réaction tangetielle du suppport.
    float frottement = 0.001f;
    m_particle->setVelocity((1.0f - frottement) * m_particle->getVelocity());
    //On enregistre la normale au sol lors de la collision pour orienter le skieur.
    glm::vec3 normal = m_plane->normal();
    normal = glm::normalize(normal);
    float anglex = acos(glm::dot(normal,glm::vec3(0,1,0))) - rotation[0];
    float angley = acos(glm::dot(normal,glm::vec3(0,0,1))) - rotation[1];
    m_particle->setRotation(m_particle->getRotation() + 0.1f*glm::vec3(anglex-1.570796f, angley, 0));
}



bool testParticleSkieurPlane(const ParticleSkieurPtr &particle, const PlanePtr &plane)
{
    if (std::abs(glm::dot(particle->getPosition(), plane->normal())-plane->distanceToOrigin()) <= particle->getRadius()) {
        particle->setCollision(true);
        return true;
    }
    particle->setCollision(false);

    return false;
}
