/* 
 * File:   SkieurRenderable.hpp
 * Author: buthodgt
 *
 * Created on March 30, 2017, 5:48 PM
 */

#ifndef SKIEURRENDERABLE_HPP
#define	SKIEURRENDERABLE_HPP

#include "../include/HierarchicalRenderable.hpp"
#include "../include/lighting/Material.hpp"
#include "../include/dynamics/Particle.hpp"
#include "../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/ControlledSkieur.hpp"
#include "../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../include/dynamics/DampingForceField.hpp"
#include "../include/texturing/BodyRenderable.hpp"
#include "../include/texturing/BottomLegRenderable.hpp"
#include "../include/texturing/TopLegRenderable.hpp"
#include "../include/texturing/SkiRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class SkieurRenderable : public HierarchicalRenderable
{
public :
    ~SkieurRenderable();
    SkieurRenderable(ShaderProgramPtr shaderProgram,
            HierarchicalRenderablePtr parentRenderable, ParticlePtr particle);
    void initControlledSkieur(ShaderProgramPtr shaderProgram, 
        HierarchicalRenderablePtr parentRenderable);

private:
    void do_draw();
    void do_animate(float time);
    void do_keyPressedEvent(sf::Event& e);

    ParticlePtr m_particle;
    ControlledSkieurPtr m_controlledSkieur;
    
    BodyRenderablePtr m_body;
    TopLegRenderablePtr m_topLeg1, m_topLeg2;
    BottomLegRenderablePtr m_botLeg1, m_botLeg2;
    SkiRenderablePtr m_ski1, m_ski2;
    
};

typedef std::shared_ptr<SkieurRenderable> SkieurRenderablePtr;

#endif	/* SKIEURRENDERABLE_HPP */

