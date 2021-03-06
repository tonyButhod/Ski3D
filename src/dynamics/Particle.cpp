#include "./../../include/dynamics/Particle.hpp"

bool Particle::isFixed() const
{
    return m_isFixed;
}

void Particle::setFixed(bool isFixed)
{
    m_isFixed = isFixed;
}

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity,
                   const float &mass, const float &radius)
    : m_initialPosition( position ), m_initialVelocity( velocity ),
    m_position(position),
    m_velocity(velocity),
    m_force(glm::vec3(0.0,0.0,0.0)),
    m_mass(mass),
    m_radius(radius), 
    m_isFixed( false ),
    m_collision( true )
{}

Particle::~Particle()
{}


const glm::vec3 & Particle::getPosition() const
{
    return m_position;
}

const glm::vec3 & Particle::getVelocity() const
{
    return m_velocity;
}

const glm::vec3 & Particle::getForce() const
{
    return m_force;
}

float Particle::getMass() const
{
    return m_mass;
}

float Particle::getRadius() const
{
    return m_radius;
}
bool Particle::getCollision() const {
    return m_collision;
}

const glm::vec3 & Particle::getRotation() const {
    return m_rotation;
}

void Particle::setPosition(const glm::vec3 &pos)
{
    m_position = pos;
}

void Particle::setVelocity(const glm::vec3 &vel)
{
    m_velocity = vel;
}

void Particle::setForce(const glm::vec3 &force)
{
    m_force = force;
}

void Particle::setRadius(const float &radius)
{
    m_radius = radius;
}

void Particle::setCollision(const bool collision) {
    m_collision = collision;
}

void Particle::setRotation(const glm::vec3 &rotation) {
    m_rotation = rotation;
}

void Particle::incrPosition(const glm::vec3 &pos)
{
    m_position += pos;
}

void Particle::incrVelocity(const glm::vec3 &vel)
{
    m_velocity += vel;
}

void Particle::incrForce(const glm::vec3& force)
{
    m_force +=force;
}

void Particle::restart()
{
    m_position = m_initialPosition;
    m_velocity = m_initialVelocity;
}

std::ostream& operator<<(std::ostream& os, const ParticlePtr& p)
{
    const glm::vec3& x = p->getPosition();
    const glm::vec3& v = p->getVelocity();

    os << "pos (" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    os << " ; ";
    os << "vel (" << v[0] << ", " << v[1] << ", " << v[2] << ")";

    return os;
}

void Particle::initParticle(glm::vec3 pos) {
    setPosition(pos + m_initPos);
    setVelocity(m_initVelocity);
    m_clock = clock();
}

void Particle::setInitPos(glm::vec3 pos){
    m_initPos = pos;
}
    
void Particle::setInitVelocity(glm::vec3 vel) {
    m_initVelocity = vel;
}

clock_t Particle::getClock() {
    return m_clock;
}