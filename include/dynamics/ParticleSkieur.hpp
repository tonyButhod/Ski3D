/* 
 * File:   ParticleSkieur.hpp
 * Author: buthodgt
 *
 * Created on April 5, 2017, 9:23 AM
 */

#ifndef PARTICLESKIEUR_HPP
#define	PARTICLESKIEUR_HPP

#include "./Particle.hpp"

class ParticleSkieur: public Particle {
    
public:
    ParticleSkieur(const glm::vec3& position, const glm::vec3& velocity,
            const float& mass, const float& radius);
    virtual ~ParticleSkieur();
    
private:
};

typedef std::shared_ptr<ParticleSkieur> ParticleSkieurPtr;

#endif	/* PARTICLESKIEUR_HPP */

