#include "../include/dynamics_rendering/SnowEffect.hpp"

#define PI 3.14159265

SnowEffect::SnowEffect(DynamicSystemPtr& system, ShaderProgramPtr& flatShader) :
	m_exist(false), m_impactpoint(glm::vec3(0.0,0.0,0.0)), m_timestart(0.0)
{
	m_system = system;
	m_shader = flatShader;
}

SnowEffect::~SnowEffectRenderable()
{}

void compute(glm::vec3 impactpoint, ParticleListRenderablePtr& effect) {
	
	float x = impactpoint.x;
	float y = impactpoint.y;
	float z = impactpoint.z;
	m_timestart = ((float)std::clock())/CLOCKS_PER_SEC;
	std::vector<ParticlePtr> particles;
	particles.resize(3*50);

	ParticlePtr p;
	float dtheta = 0;
	for (int j=0; j<3; ++j) {
		for (int i=0; i<50; ++i) {
			p = std::make_shared<Particle>(
					glm::vec3(x+sin(dtheta)+j*0.01,y+cos(dtheta)+j*0.01, z),
				   	glm::vec3(sin(dtheta)*(rand()%100)/100, cos(dtheta)*(rand()%100)/100, 1.0),
				   	1, 0.01);
			dtheta += 2*PI/50;
			particles[50*j + i](p);
			m_system->addParticle(p)
		}
		dtheta = 0;
	}

	effect = std::make_shared<ParticleListRenderable>(m_shader, particles); 

}

void do_animate(float time)
{
	float current_time = ((float)std::clock())/CLOCKS_PER_SEC;
	if (current_time - m_timestart >= 2) {
		effect = std::make_shared<ParticleListRenderable>(m_shader, NULL);	
	} else {
		
	}
}
