#include "../include/SkieurRenderable.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

SkieurRenderable::SkieurRenderable(ShaderProgramPtr shaderProgram, ParticlePtr particle,
            HierarchicalRenderablePtr parentRenderable, DynamicSystemPtr system)
    : HierarchicalRenderable(shaderProgram), m_particle(particle)
{
    glm::mat4 parentTransform(1.0);
    MaterialPtr normalMat = Material::Normal();
    
    //Body
    BodyRenderablePtr texBody = std::make_shared<BodyRenderable>(shaderProgram, particle);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 4.0, 0.0));
    texBody->setParentTransform(parentTransform);
    texBody->setMaterial(normalMat);
    HierarchicalRenderable::addChild(parentRenderable, texBody);
    //Jambe 1
    TopLegRenderablePtr texTopLeg1 = std::make_shared<TopLegRenderable>(shaderProgram);
    texTopLeg1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.6,0.0,0.0));
    texTopLeg1->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(texBody, texTopLeg1);
    
    BottomLegRenderablePtr texBotLeg1 = std::make_shared<BottomLegRenderable>(shaderProgram);
    texBotLeg1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,-2.0,0.0));
    texBotLeg1->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(texTopLeg1, texBotLeg1);
    //Jambe 2
    TopLegRenderablePtr texTopLeg2 = std::make_shared<TopLegRenderable>(shaderProgram);
    texTopLeg2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(-0.6,0.0,0.0));
    texTopLeg2->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(texBody, texTopLeg2);
    
    BottomLegRenderablePtr texBotLeg2 = std::make_shared<BottomLegRenderable>(shaderProgram);
    texBotLeg2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,-2.0,0.0));
    texBotLeg2->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(texTopLeg2, texBotLeg2);
    
    //Initialize a force field that apply only to the mobile particle
    glm::vec3 nullForce(0.0, 0.0, 0.0);
    std::vector<ParticlePtr> vParticle;
    vParticle.push_back(particle);
    ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
    system->addForceField(force);
    
    //Initialize a renderable for the force field applied on the mobile particle.
    //This renderable allows to modify the attribute of the force by key/mouse events
    //Add this renderable to the systemRenderable.
    ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>(shaderProgram, force);
    HierarchicalRenderable::addChild(parentRenderable, forceRenderable);
    texBody->setControlled(forceRenderable);
    
    ControlledSkieurPtr controlledSkieur = std::make_shared<ControlledSkieur>(shaderProgram);
    HierarchicalRenderable::addChild(parentRenderable, controlledSkieur);
    texBody->setControlledSkieur(controlledSkieur);
    texTopLeg1->setControlledSkieur(controlledSkieur);
    texTopLeg2->setControlledSkieur(controlledSkieur);
    texBotLeg1->setControlledSkieur(controlledSkieur);
    texBotLeg2->setControlledSkieur(controlledSkieur);
    
    //Add a damping force field to the mobile.
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
    system->addForceField(dampingForceField);
}

SkieurRenderable::~SkieurRenderable()
{
}

void SkieurRenderable::do_draw()
{
}

void SkieurRenderable::do_animate(float time)
{
}

void SkieurRenderable::do_keyPressedEvent(sf::Event& e) {
    
}