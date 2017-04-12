#include "./../../include/dynamics_rendering/ControlledSkieur.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"

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
    angularSpeed = glm::vec3(4.0,4.0,2.0);
    
    enter = false;
    left = false;
    right = false;
    front = false;
    back = false;
    lincolnL = false;
    lincolnR = false;
}

ControlledSkieur::ControlledSkieur(ShaderProgramPtr program, ParticleSkieurPtr particle)
    : HierarchicalRenderable(program), m_particle(particle)
{
    m_status = ControlledSkieurStatus();
}

ControlledSkieur::~ControlledSkieur()
{
}

void ControlledSkieur::do_keyPressedEvent(sf::Event& e)
{
    if (e.key.code == sf::Keyboard::Return) {
        m_status.enter = true;
    } else if (e.key.code == sf::Keyboard::Left) {
        m_status.left = true;
    } else if (e.key.code == sf::Keyboard::Right) {
        m_status.right = true;
    } else if (e.key.code == sf::Keyboard::Up) {
        m_status.front = true;
    } else if (e.key.code == sf::Keyboard::Down) {
        m_status.back = true;
    } else if (e.key.code == sf::Keyboard::J) {
        m_status.lincolnL = true;
    } else if (e.key.code == sf::Keyboard::K) {
        m_status.lincolnR = true;
    }
}

void ControlledSkieur::do_keyReleasedEvent(sf::Event& e)
{
    if (e.key.code == sf::Keyboard::Return) {
        m_status.enter = false;
    } else if (e.key.code == sf::Keyboard::Left) {
        m_status.left = false;
    } else if (e.key.code == sf::Keyboard::Right) {
        m_status.right = false;
    } else if (e.key.code == sf::Keyboard::Up) {
        m_status.front = false;
    } else if (e.key.code == sf::Keyboard::Down) {
        m_status.back = false;
    } else if (e.key.code == sf::Keyboard::J) {
        m_status.lincolnL = false;
    } else if (e.key.code == sf::Keyboard::K) {
        m_status.lincolnR = false;
    }
}

void ControlledSkieur::do_animate(float time)
{
    if (time > m_status.last_time) {
        float dt = time - m_status.last_time;
        if (m_status.enter) {
            m_status.angle += dt * m_status.angularSpeedDown;
            if (m_status.angle > m_status.max_angle)
                m_status.angle = m_status.max_angle;
        }
        else {
            m_status.angle -= dt * m_status.angularSpeedUp;
            if (m_status.angle < m_status.min_angle)
                m_status.angle = m_status.min_angle;
            if ((m_particle->getCollision() || m_particle->getJumpCollision()) && m_status.angle != m_status.min_angle) {
                glm::vec3 velocity = m_particle->getVelocity();
                velocity[2] = m_status.angle*5.0;
                m_particle->setVelocity(velocity);
            }
        }
        if (m_status.left && !m_status.right) {
            glm::vec3 rot = m_particle->getRotation();
            rot[2] += dt * m_status.angularSpeed[2];
            m_particle->setRotation(rot);
        } else if (m_status.right && !m_status.left) {
            glm::vec3 rot = m_particle->getRotation();
            rot[2] -= dt * m_status.angularSpeed[2];
            m_particle->setRotation(rot);
        }

        if (!m_particle->getCollision() && !m_particle->getJumpCollision()) {
            if (m_status.front && !m_status.back) {
                glm::vec3 rot = m_particle->getRotation();
                rot[1] += cos(rot[2]) * dt * m_status.angularSpeed[1];
                rot[0] += -sin(rot[2]) * dt * m_status.angularSpeed[1];
                m_particle->setRotation(rot);
            } else if (m_status.back && !m_status.front) {
                glm::vec3 rot = m_particle->getRotation();
                rot[1] -= cos(rot[2]) * dt * m_status.angularSpeed[1];
                rot[0] -= -sin(rot[2]) * dt * m_status.angularSpeed[1];
                m_particle->setRotation(rot);
            }

            if (m_status.lincolnR && !m_status.lincolnL) {
                glm::vec3 rot = m_particle->getRotation();
                rot[1] += -sin(rot[2]) * dt * m_status.angularSpeed[0];
                rot[0] += cos(rot[2]) * dt * m_status.angularSpeed[0];
                m_particle->setRotation(rot);
            } else if (m_status.lincolnL && !m_status.lincolnR) {
                glm::vec3 rot = m_particle->getRotation();
                rot[1] -= -sin(rot[2]) * dt * m_status.angularSpeed[0];
                rot[0] -= cos(rot[2]) * dt * m_status.angularSpeed[0];
                m_particle->setRotation(rot);
            }
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
