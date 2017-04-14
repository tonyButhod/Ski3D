#include "./../../include/dynamics/ReactionTangentielle.hpp"

ReactionTangentielle::ReactionTangentielle(const std::vector<ParticleSkieurPtr> particles, const float damping)
{
    m_particles = particles;
    m_damping = damping;
}

ReactionTangentielle::~ReactionTangentielle()
{
}

void ReactionTangentielle::do_addForce()
{
    for (ParticleSkieurPtr p : m_particles) {
        if (p->getCollision() || p->getJumpCollision())
            p->incrForce(-m_damping*p->getVelocity());
    }
}

const std::vector<ParticleSkieurPtr> ReactionTangentielle::getParticles()
{
    return m_particles;
}

void ReactionTangentielle::setParticles(const std::vector<ParticleSkieurPtr>& particles)
{
    m_particles = particles;
}

const float& ReactionTangentielle::getDamping()
{
    return m_damping;
}

void ReactionTangentielle::setDamping(const float& damping)
{
    m_damping = damping;
}
