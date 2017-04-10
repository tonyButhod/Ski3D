#ifndef SKIEUR_COLLISION_HPP
#define SKIEUR_COLLISION_HPP

#include "Collision.hpp"
#include "ParticleSkieur.hpp"
#include "Plane.hpp"

/**@brief Implement the resolution of a collision event between a plane and a particleSkieur.
 *
 * Implementation of the resolution of a collision event between a plane and a particleSkieur.
 */
class SkieurCollision : public Collision
{
public:
    SkieurCollision(ParticleSkieurPtr particle, PlanePtr plane, float restitution);

    virtual ~SkieurCollision();

private:
    void do_solveCollision();

    ParticleSkieurPtr m_particle;
    PlanePtr m_plane;
};

typedef std::shared_ptr<SkieurCollision> SkieurCollisionPtr;

bool testParticleSkieurPlane(const ParticleSkieurPtr& particle, const PlanePtr& plane);

#endif //SKIEUR_COLLISION_HPP
