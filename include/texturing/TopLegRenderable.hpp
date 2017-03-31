/* 
 * File:   TopLegRenderable.hpp
 * Author: buthodgt
 *
 * Created on March 27, 2017, 11:54 AM
 */

#ifndef TOPLEGRENDERABLE_HPP
#define	TOPLEGRENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "../../include/dynamics_rendering/ControlledSkieur.hpp"
#include <vector>
#include <glm/glm.hpp>

class TopLegRenderable : public HierarchicalRenderable
{
public :
    ~TopLegRenderable();
    TopLegRenderable(ShaderProgramPtr shaderProgram);
    void setMaterial(const MaterialPtr& material);
    void setControlledSkieur(ControlledSkieurPtr controlledSkieur);
    void setPosRepos(glm::mat4 pos);

private:
    void do_draw();
    void do_animate(float time);
    void do_keyPressedEvent(sf::Event& e);

    ControlledSkieurPtr m_controlledSkieur;
    glm::mat4 m_posRepos = glm::mat4(1.0);
    
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

typedef std::shared_ptr<TopLegRenderable> TopLegRenderablePtr;

#endif	/* TOPLEGRENDERABLE_HPP */

