/* 
 * File:   JumpRenderable.hpp
 * Author: buthodgt
 *
 * Created on March 27, 2017, 11:54 AM
 */

#ifndef JUMPRENDERABLE_HPP
#define	JUMPRENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class JumpRenderable : public HierarchicalRenderable
{
public :
    ~JumpRenderable();
    JumpRenderable(ShaderProgramPtr shaderProgram);
    void setMaterial(const MaterialPtr& material);
    void setCornerMin(glm::vec3 &corner);
    void setCornerMax(glm::vec3 &corner);
    void setNormal(glm::vec3 &normal);
    const glm::vec3 & getCornerMax();
    const glm::vec3 & getCornerMin();
    const glm::vec3 & getNormal();
private:
    void do_draw();
    void do_animate(float time);
    void do_keyPressedEvent(sf::Event& e);
    
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
    glm::vec3 m_cornerMin;
    glm::vec3 m_cornerMax;
    glm::vec3 m_n;
};

typedef std::shared_ptr<JumpRenderable> JumpRenderablePtr;

void constructJump(std::vector<glm::vec3> &pos, std::vector<glm::vec3> &norm, 
        std::vector<glm::vec2> &tex);

#endif	/* JUMPRENDERABLE_HPP */

