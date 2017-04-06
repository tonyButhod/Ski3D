#include "./../../include/dynamics/EulerExplicitSolver.hpp"

EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<ParticlePtr>& particles)
{
    for (ParticlePtr p : particles) {
        if (!p->isFixed()) {
            //Implement explicit euler solver
            //Functions to use:
            //Particle::getPosition(), Particle::getVelocity(), Particle::getMass(), Particle::getForce()
            //Particle::setPosition(), Particle::setVelocity()
            p->setVelocity(p->getVelocity()+dt/p->getMass()*p->getForce());
            p->setPosition(p->getPosition()+dt*p->getVelocity());
        }
    }
}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<ParticleSkieurPtr>& particles)
{
    for (ParticlePtr p : particles) {
        if (!p->isFixed()) {
            //Implement explicit euler solver
            //Functions to use:
            //Particle::getPosition(), Particle::getVelocity(), Particle::getMass(), Particle::getForce()
            //Particle::setPosition(), Particle::setVelocity()
            p->setVelocity(p->getVelocity()+dt/p->getMass()*p->getForce());
            p->setPosition(p->getPosition()+dt*p->getVelocity());
        }
    }
}
