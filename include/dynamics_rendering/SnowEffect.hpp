#ifndef SNOWEFFECT_HPP
#define SNOWEFFECT_HPP 

#include "../../include/dynamics_rendering/ParticleListRenderable.hpp"
#include "DynamicSystemRenderable.hpp"
#include "../../include/dynamics/Particle.hpp"

#include <time.h>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class SnowEffect
{
public:

	SnowEffect(DynamicSystemPtr system);
	~SnowEffect();
	void compute(glm::vec3 impactpoint, ParticleListRenderablePtr& effect);
	void do_animate(float time);

private:

	bool m_exist;
	float m_timestart;
	glm::vec4 m_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glm::vec3 m_impactpoint;
	DynamicSystemPtr m_system;
	ShaderProgramPtr m_shader;

};

typedef std::shared_ptr<SnowEffect> SnowEffectPtr

#endif /* SNOWEFFECT_HPP */
