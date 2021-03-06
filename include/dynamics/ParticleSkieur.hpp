/* 
 * File:   ParticleSkieur.hpp
 * Author: buthodgt
 *
 * Created on April 5, 2017, 9:23 AM
 */

#ifndef PARTICLESKIEUR_HPP
#define	PARTICLESKIEUR_HPP

#include "./Particle.hpp"
#include "../texturing/JumpRenderable.hpp"

class ParticleSkieur: public Particle {
    
public:
    ParticleSkieur(const glm::vec3& position, const glm::vec3& velocity,
            const float& mass, const float& radius);
    virtual ~ParticleSkieur();
    
    void setJumpCollision(const bool jumpCollision);
    bool getJumpCollision();
    void setJump(const bool jump);
    bool canJump();
    void setFail(bool fail);
    bool fail();
    
    void setJump(const JumpRenderablePtr jump);
    JumpRenderablePtr getJump();
    
    void applyEffect();
    void setEffects(std::vector<ParticlePtr> list);
    
    
private:
    bool m_jumpCollision;
    bool m_canJump;
    bool m_fail;
    JumpRenderablePtr m_jump;
    std::vector<ParticlePtr> m_effects;
};

typedef std::shared_ptr<ParticleSkieur> ParticleSkieurPtr;

#endif	/* PARTICLESKIEUR_HPP */

