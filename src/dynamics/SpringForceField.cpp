#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2,
                                   float stiffness, float equilibriumLength,
                                   float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness),
    m_equilibriumLength(equilibriumLength),
    m_damping(damping)
{}

void SpringForceField::do_addForce()
{
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless
    float dist = (float) glm::length(m_p1->getPosition()-m_p2->getPosition());
    glm::vec3 norm = glm::normalize(m_p1->getPosition()-m_p2->getPosition());
    if (std::abs(dist - m_equilibriumLength) <= std::numeric_limits<float>::epsilon())
        return;
    glm::vec3 springforce(-m_stiffness*(dist-m_equilibriumLength)*norm);
    
    springforce += -m_damping*glm::dot(m_p1->getVelocity()-m_p2->getVelocity(), norm)*norm;
    
    m_p1->setForce(m_p1->getForce()+springforce);
    m_p2->setForce(m_p2->getForce()-springforce);
}

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
