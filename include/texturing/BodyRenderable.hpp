/* 
 * File:   BodyRenderable.hpp
 * Author: buthodgt
 *
 * Created on March 27, 2017, 11:54 AM
 */

#ifndef BODYRENDERABLE_HPP
#define	BODYRENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "../dynamics/Particle.hpp"
#include "../dynamics/ParticleSkieur.hpp"
#include "../../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"
#include "../../include/dynamics_rendering/ControlledSkieur.hpp"
#include <vector>
#include <glm/glm.hpp>

class BodyRenderable : public HierarchicalRenderable
{
public :
    ~BodyRenderable();
    BodyRenderable(ShaderProgramPtr shaderProgram);
    void setMaterial(const MaterialPtr& material);
    void setControlledSkieur(ControlledSkieurPtr controlledSkieur);
    void setParticle(ParticleSkieurPtr particle);

private:
    void do_draw();
    void do_animate(float time);
    void do_keyPressedEvent(sf::Event& e);

    ParticleSkieurPtr m_particle;
    ControlledSkieurPtr m_controlledSkieur;
        
    size_t m_numberOfVertices;
    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec2 > m_texCoords;
    std::vector< glm::vec2 > m_origTexCoords;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    unsigned int m_wrapOption;
    unsigned int m_filterOption;

    MaterialPtr m_material;
};

typedef std::shared_ptr<BodyRenderable> BodyRenderablePtr;

#endif	/* BODYRENDERABLE_HPP */

