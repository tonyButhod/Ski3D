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
#include <vector>
#include <glm/glm.hpp>

class SkieurRenderable : public HierarchicalRenderable
{
public :
    ~SkieurRenderable();
    SkieurRenderable(ShaderProgramPtr shaderProgram, ParticlePtr particle,
            HierarchicalRenderablePtr parentRenderable, DynamicSystemPtr system);

private:
    void do_draw();
    void do_animate(float time);
    void do_keyPressedEvent(sf::Event& e);

    ParticlePtr m_particle;
    ControlledForceFieldRenderablePtr m_controlled;
    ControlledSkieurPtr m_controlledSkieur;
    
};

typedef std::shared_ptr<SkieurRenderable> SkieurRenderablePtr;

#endif	/* SKIEURRENDERABLE_HPP */

