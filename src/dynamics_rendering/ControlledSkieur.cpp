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
    max_angle = 1.2f;
    angularSpeedDown = 4.0;
    angularSpeedUp = 5.5;
    
    space = false;
}

ControlledSkieur::ControlledSkieur(ShaderProgramPtr program)
    : HierarchicalRenderable(program),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    m_status = ControlledSkieurStatus();

    //Create geometric data to display an arrow representing the movement of the particle
    m_positions.clear();
    m_colors.clear();
    m_normals.clear();

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

ControlledSkieur::~ControlledSkieur()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
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
