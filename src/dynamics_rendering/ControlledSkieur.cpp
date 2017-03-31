#include "./../../include/dynamics_rendering/ControlledSkieur.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ControlledSkieurStatus::ControlledSkieurStatus()
{
    clear();
}

ControlledSkieurStatus::~ControlledSkieurStatus()
{}

void ControlledSkieurStatus::clear()
{
    angle =  0;
    last_time =  0;
    intensity = 0;
    min_angle = 0.0f;
    max_angle = 0.9f;
    angularSpeedDown = 4.0;
    angularSpeedUp = 5.5;
    
    space = false;
}

ControlledSkieur::ControlledSkieur(ShaderProgramPtr program)
    : HierarchicalRenderable(program)
{
    m_status = ControlledSkieurStatus();
}

ControlledSkieur::~ControlledSkieur()
{
}

void ControlledSkieur::do_keyPressedEvent(sf::Event& e)
{
    if (e.key.code == sf::Keyboard::Return) {
        m_status.space = true;
    }
}

void ControlledSkieur::do_keyReleasedEvent(sf::Event& e)
{
    if (e.key.code == sf::Keyboard::Return) {
        m_status.space = false;
    }
}

void ControlledSkieur::do_animate(float time)
{
    if (time > m_status.last_time) {
        float dt = time - m_status.last_time;
        if (m_status.space) {
            m_status.angle += dt * m_status.angularSpeedDown;
            if (m_status.angle > m_status.max_angle)
                m_status.angle = m_status.max_angle;
        }
        else {
            m_status.angle -= dt * m_status.angularSpeedUp;
            if (m_status.angle < m_status.min_angle)
                m_status.angle = m_status.min_angle;
        }
    }
    m_status.last_time = time;
}

void ControlledSkieur::do_draw()
{
}

const float& ControlledSkieur::getAngle() {
    return m_status.angle;
}
