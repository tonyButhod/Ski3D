#ifndef BANNERRENDERABLE_HPP
#define BANNERRENDERABLE_HPP

#include "../include/Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

#define PI 3.14159265

#define LENGTH 8
#define WIDTH 6

#define LENGTH_SUBDIVISIONS 10
#define WIDTH_SUBDIVISIONS 10

class BannerRenderable : public Renderable
{
    public:
        ~BannerRenderable();
        BannerRenderable(ShaderProgramPtr program);

    protected:
        void do_draw();
        void do_animate(float time);

    private:
        bool m_normalPerVertex;
        void buildNormalPerVertex(unsigned int nbSlices);
        void buildNormalPerFace(unsigned int nbSlices);

        std::vector< glm::vec3 > m_positions;
		std::vector< glm::vec3 > m_pole_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

		int start_index;

		float m_time; 

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<BannerRenderable> BannerRenderablePtr;
#endif
