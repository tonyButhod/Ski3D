/* 
 * File:   Fence.hpp
 * Author: buthodgt
 *
 * Created on March 27, 2017, 11:54 AM
 */

#ifndef FENCE_HPP
#define	FENCE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class Fence : public HierarchicalRenderable
{
public :
    ~Fence();
    Fence(ShaderProgramPtr shaderProgram);
    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate(float time);
    void do_keyPressedEvent(sf::Event& e);
    
    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec3 > m_normals;
    std::vector<unsigned int> m_indices;
    std::vector< glm::vec2 > m_texCoords;
    std::vector< glm::vec2 > m_origTexCoords;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_iBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    unsigned int m_wrapOption;
    unsigned int m_filterOption;

    MaterialPtr m_material;
};

typedef std::shared_ptr<Fence> FencePtr;

void addFences(HierarchicalRenderablePtr system, ShaderProgramPtr texShader, int nb_fences, glm::mat4 transform);

#endif	/* FENCE_HPP */

