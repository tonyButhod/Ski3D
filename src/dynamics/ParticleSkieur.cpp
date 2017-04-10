#include "../../include/dynamics/ParticleSkieur.hpp"

ParticleSkieur::ParticleSkieur(const glm::vec3 &position, const glm::vec3 &velocity,
                   const float &mass, const float &radius) 
    : Particle(position, velocity, mass, radius) {
    
}

ParticleSkieur::~ParticleSkieur() {
    
}