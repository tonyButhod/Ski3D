#ifndef REACTION_TANGENTIELLE_HPP
#define REACTION_TANGENTIELLE_HPP

#include <vector>
#include "ForceField.hpp"
#include "ParticleSkieur.hpp"

/**@brief Implement a damping force field.
 *
 * This class implement a force field that models a damping, i.e.
 * a kinetic friction.
 */
class ReactionTangentielle : public ForceField
{
public:
    /**@brief Build a damping force field.
     *
     * Build a damping force applied to a set of particles. The force added
     * to a particle of velocity v is -damping * v.
     * @param particles Set of particles influenced by this damping force.
     * @param damping Damping coefficient. */
    ReactionTangentielle(const std::vector<ParticleSkieurPtr> particles, const float damping);
    virtual ~ReactionTangentielle();

    /**@brief Access to the particles influenced by this force field.
     *
     * Get the particles influenced by this damping force field.
     * @return The set of particles influenced by this.
     */
    const std::vector<ParticleSkieurPtr> getParticles();

    /**@brief Define the set of particles influenced by this force field.
     *
     * Define the set of particles that will be influenced by this damping
     * force force.
     * @param particles The new set of influenced particles.
     */
    void setParticles(const std::vector<ParticleSkieurPtr>& particles);

    /**@brief Access to the damping factor.
     *
     * Get the damping factor of this force field.
     * @return The damping factor of this.
     */
    const float& getDamping();

    /**@brief Set the damping factor of this force field.
     *
     * Define the damping factor of this damping force field.
     * @param damping The new damping factor.
     */
    void setDamping(const float& damping);

private:
    void do_addForce();
    std::vector<ParticleSkieurPtr> m_particles;
    float m_damping;
};

typedef std::shared_ptr<ReactionTangentielle> ReactionTangentiellePtr;

#endif // REACTION_TANGENTIELLE_HPP
