#include "../include/SkieurRenderable.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

SkieurRenderable::SkieurRenderable(ShaderProgramPtr shaderProgram,
            HierarchicalRenderablePtr parentRenderable, ParticlePtr particle)
    : HierarchicalRenderable(shaderProgram), m_particle(particle)
{
    glm::mat4 parentTransform(1.0);
    MaterialPtr normalMat = Material::Normal();
    
    //Body
    m_body = std::make_shared<BodyRenderable>(shaderProgram);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 4.0));
    m_body->setParentTransform(parentTransform);
    m_body->setMaterial(normalMat);
    HierarchicalRenderable::addChild(parentRenderable, m_body);
    m_body->setParticle(m_particle);
    //Jambe 1
    m_topLeg1 = std::make_shared<TopLegRenderable>(shaderProgram);
    m_topLeg1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.6,0.0,0.0));
    m_topLeg1->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(m_body, m_topLeg1);
    
    m_botLeg1 = std::make_shared<BottomLegRenderable>(shaderProgram);
    m_botLeg1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-2.0));
    m_botLeg1->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(m_topLeg1, m_botLeg1);
    //Jambe 2
    m_topLeg2 = std::make_shared<TopLegRenderable>(shaderProgram);
    m_topLeg2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(-0.6,0.0,0.0));
    m_topLeg2->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(m_body, m_topLeg2);
    
    m_botLeg2 = std::make_shared<BottomLegRenderable>(shaderProgram);
    m_botLeg2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-2.0));
    m_botLeg2->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(m_topLeg2, m_botLeg2);
    
    m_ski1 = std::make_shared<SkiRenderable>(shaderProgram);
    m_ski1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-2.0));
    m_ski1->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(m_botLeg1, m_ski1);
    
    m_ski2 = std::make_shared<SkiRenderable>(shaderProgram);
    m_ski2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-2.0));
    m_ski2->setPosRepos(parentTransform);
    HierarchicalRenderable::addChild(m_botLeg2, m_ski2);
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

void SkieurRenderable::initControlledSkieur(ShaderProgramPtr shaderProgram, 
        HierarchicalRenderablePtr parentRenderable) {
    m_controlledSkieur = std::make_shared<ControlledSkieur>(shaderProgram, m_particle);
    HierarchicalRenderable::addChild(parentRenderable, m_controlledSkieur);
    m_body->setControlledSkieur(m_controlledSkieur);
    m_topLeg1->setControlledSkieur(m_controlledSkieur);
    m_topLeg2->setControlledSkieur(m_controlledSkieur);
    m_botLeg1->setControlledSkieur(m_controlledSkieur);
    m_botLeg2->setControlledSkieur(m_controlledSkieur);
    m_botLeg1->setControlledSkieur(m_controlledSkieur);
    m_botLeg2->setControlledSkieur(m_controlledSkieur);
    m_ski1->setControlledSkieur(m_controlledSkieur);
    m_ski2->setControlledSkieur(m_controlledSkieur);
}