#ifndef TEXTURED_SAPIN_RENDERABLE_HPP
#define TEXTURED_SAPIN_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class TexturedSapinRenderable : public HierarchicalRenderable
{
public :
    ~TexturedSapinRenderable();
    TexturedSapinRenderable(ShaderProgramPtr shaderProgram, const std::string& textureFilename);
    void create_leaf(glm::mat4 &transfo);
    void setMaterial(const MaterialPtr& material);

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
};
void getUnitCone(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords, 
        const unsigned int& strips, const unsigned int& slices);

typedef std::shared_ptr<TexturedSapinRenderable> TexturedSapinRenderablePtr;

#endif
