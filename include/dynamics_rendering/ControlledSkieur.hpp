#ifndef CONTROLLED_SKIEUR_HPP
#define CONTROLLED_SKIEUR_HPP

#include "../HierarchicalRenderable.hpp"
#include "../dynamics/ConstantForceField.hpp"
#include "../dynamics/ParticleSkieur.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

/**@brief Status of a ControlledSkieurStatus.
 *
 * This status holds important variables that are used to control a force
 * field thanks to user inputs. You can (and should) adapt this class
 * and its default value to the kind of behavior you want in your project.
 */
class ControlledSkieurStatus
{
public:
    ControlledSkieurStatus();
    ~ControlledSkieurStatus();
    void clear();

public:
    float angle;
    float last_time;
    float intensity;
    float min_angle;
    float max_angle;
    float angularSpeedDown;
    float angularSpeedUp;
    glm::vec3 angularSpeed;

    bool enter, left, right, front, back, lincolnR, lincolnL;
};


/**@brief Implement a force field controlled by user input.
 *
 * This class is an example of what you could do to control a dynamic component
 * (e.g. the kart in your project) thanks to user inputs. This is done by modifying
 * the force of a constant force field applied to the mobile(s) you want to control.
 *
 * We derive from a renderable to be able to react to user input. This might look
 * nasty, but since you dealt with renderables during all previous praticales, we think
 * this is the easiest way to do so. We derive from a hierarchical renderable to be able
 * to use the same local frame as the dynamic system using the force field (see DynamicSystemRenderable).
 */
class ControlledSkieur : public HierarchicalRenderable
{
public:
  ControlledSkieur(ShaderProgramPtr program, ParticleSkieurPtr particle);
  ~ControlledSkieur();
  const float &getAngle();

private:

  virtual void do_keyPressedEvent(sf::Event& e);
  virtual void do_keyReleasedEvent(sf::Event& e);
  virtual void do_animate(float time);
  virtual void do_draw();

  ControlledSkieurStatus m_status;
  ParticleSkieurPtr m_particle;
};

typedef std::shared_ptr<ControlledSkieur> ControlledSkieurPtr;

#endif //CONTROLLED_SKIEUR_HPP
