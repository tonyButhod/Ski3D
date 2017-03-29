#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/lighting/PointLightRenderable.hpp"
#include "../include/lighting/SpotLightRenderable.hpp"
#include "../include/lighting/LightedCubeRenderable.hpp"
#include "../include/lighting/LightedCylinderRenderable.hpp"
#include "../include/lighting/LightedMeshRenderable.hpp"

#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"
#include "../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"

#include "../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderable.hpp"
#include "../include/texturing/BottomLegRenderable.hpp"
#include "../include/texturing/TopLegRenderable.hpp"
#include "../include/texturing/BodyRenderable.hpp"


void initialize_practical_08_scene(Viewer& viewer)
{
    // create all shaders of this scene, then add them to the viewer
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                          "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

    // Two texture shaders: simple and multi-texturing
    ShaderProgramPtr texShader
        = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl",
                                          "../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    ShaderProgramPtr multiTexShader
        = std::make_shared<ShaderProgram>("../shaders/multiTextureVertex.glsl",
                                          "../shaders/multiTextureFragment.glsl");
    viewer.addShaderProgram(multiTexShader);

    ShaderProgramPtr phongShader
        = std::make_shared<ShaderProgram>("../shaders/phongVertex.glsl",
                                          "../shaders/phongFragment.glsl");
    viewer.addShaderProgram(phongShader);

    //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);
    
    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep()in the animate() function
    //It also handles some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    //Define a transformation
    glm::mat4 parentTransform, localTransform;
    std::string filename;

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    localTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    directionalLightRenderable->setLocalTransform(localTransform);
    viewer.setDirectionalLight(directionalLight);
    viewer.addRenderable(directionalLightRenderable);

    //Define a spot light
    glm::vec3 s_position(0.0,12.0,-5.0), s_spotDirection = glm::normalize(glm::vec3(0.0,-2.0,1.0));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.5,0.5,0.5), s_specular(0.5,0.5,0.5);
    float s_constant=1.0, s_linear=0.0, s_quadratic=0.0;
    float s_cosInnerCutOff = std::cos(glm::radians(20.0f));
    float s_cosOuterCutOff = std::cos(glm::radians(40.0f));
    SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_cosInnerCutOff, s_cosOuterCutOff);
    SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight);
    localTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    spotLightRenderable->setLocalTransform(localTransform);
    viewer.addSpotLight(spotLight);
    viewer.addRenderable(spotLightRenderable);

    //Define materials
    glm::vec3 mAmbient(0.0), mDiffuse(0.0), mSpecular(0.0);
    float mShininess=0.0;
    MaterialPtr pearl = Material::Pearl();
    MaterialPtr bronze = Material::Bronze();
    MaterialPtr emerald = Material::Emerald();
    MaterialPtr normalMat = Material::Normal();

    //Initialize object with position, velocity, mass and radius and add it to the system
    glm::vec3 px(0.0, 0.0, 0.0);
    glm::vec3 pv(0.0, 0.0, 0.0);
    float pm = 1.0, pr = 1.0;
    px = glm::vec3(0.0,4.0,0.0);
    ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);
    system->addParticle( mobile );
    
    //Skieur
    BodyRenderablePtr texBody = std::make_shared<BodyRenderable>(texShader, mobile);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 4.0, 0.0));
    texBody->setParentTransform(parentTransform);
    texBody->setMaterial(normalMat);
    HierarchicalRenderable::addChild(systemRenderable, texBody);
    //viewer.addRenderable(texBody);
    //Jambe 1
    TopLegRenderablePtr texTopLeg1 = std::make_shared<TopLegRenderable>(texShader);
    texTopLeg1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.6,0.0,0.0));
    texTopLeg1->setParentTransform(parentTransform);
    HierarchicalRenderable::addChild(texBody, texTopLeg1);
    
    BottomLegRenderablePtr texBotLeg1 = std::make_shared<BottomLegRenderable>(texShader);
    texBotLeg1->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,-2.0,0.0));
    texBotLeg1->setParentTransform(parentTransform);
    HierarchicalRenderable::addChild(texTopLeg1, texBotLeg1);
    //Jambe 2
    TopLegRenderablePtr texTopLeg2 = std::make_shared<TopLegRenderable>(texShader);
    texTopLeg2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(-0.6,0.0,0.0));
    texTopLeg2->setParentTransform(parentTransform);
    HierarchicalRenderable::addChild(texBody, texTopLeg2);
    
    BottomLegRenderablePtr texBotLeg2 = std::make_shared<BottomLegRenderable>(texShader);
    texBotLeg2->setMaterial(normalMat);
    parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0,-2.0,0.0));
    texBotLeg2->setParentTransform(parentTransform);
    HierarchicalRenderable::addChild(texTopLeg2, texBotLeg2);

    //Lighted Cube
    LightedCubeRenderablePtr ground
        = std::make_shared<LightedCubeRenderable>(phongShader, pearl);
    localTransform = glm::scale(glm::mat4(1.0), glm::vec3(20.0,0.2,20.0));
    localTransform *= glm::translate(glm::mat4(1.0), glm::vec3(0.0,-0.5,0.0));
    ground->setLocalTransform(localTransform);
    ground->setMaterial(pearl);
    viewer.addRenderable(ground);
    
    //Initialize a force field that apply only to the mobile particle
    glm::vec3 nullForce(0.0, 0.0, 0.0);
    std::vector<ParticlePtr> vParticle;
    vParticle.push_back(mobile);
    ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
    system->addForceField(force);

    //Initialize a renderable for the force field applied on the mobile particle.
    //This renderable allows to modify the attribute of the force by key/mouse events
    //Add this renderable to the systemRenderable.
    ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>(flatShader, force);
    HierarchicalRenderable::addChild(systemRenderable, forceRenderable);
    texBody->setControlled(forceRenderable);
    
    ControlledSkieurPtr controlledSkieur = std::make_shared<ControlledSkieur>(flatShader);
    HierarchicalRenderable::addChild(systemRenderable, controlledSkieur);
    texBody->setControlledSkieur(controlledSkieur);
    texTopLeg1->setControlledSkieur(controlledSkieur);
    texTopLeg2->setControlledSkieur(controlledSkieur);
    texBotLeg1->setControlledSkieur(controlledSkieur);
    texBotLeg2->setControlledSkieur(controlledSkieur);

    //Add a damping force field to the mobile.
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
    system->addForceField(dampingForceField);
    
    viewer.startAnimation();
}
