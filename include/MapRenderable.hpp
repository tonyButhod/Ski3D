#ifndef MAP_RENDERABLE_HPP
#define MAP_RENDERABLE_HPP

#include "lighting/Material.hpp"
#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class MapRenderable : public Renderable
{
    public:
        ~MapRenderable();
        MapRenderable();
        MapRenderable(ShaderProgramPtr program, const std::string& textureFilename);

        void setMaterial(const MaterialPtr& material);
        void generateSapin(Viewer& viewer, ShaderProgramPtr texShader);

    private:
        void do_draw();
        void do_animate(float time);

        std::vector<glm::vec3> m_positions;
        std::vector< glm::vec3 > m_normals;
        std::vector< glm::vec2 > m_texCoords;

        unsigned int m_pBuffer;
        unsigned int m_nBuffer;
        unsigned int m_tBuffer;
        unsigned int m_texId;

        MaterialPtr m_material;
};

typedef std::shared_ptr<MapRenderable> MapRenderablePtr;

#endif
