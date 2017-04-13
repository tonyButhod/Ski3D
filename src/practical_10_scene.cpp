#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/SkieurRenderable.hpp"

#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"
#include "../include/dynamics/ParticleSkieur.hpp"

#include "../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../include/dynamics_rendering/ParticleRenderable.hpp"
#include "../include/dynamics_rendering/ParticleListRenderable.hpp"
#include "../include/dynamics_rendering/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringListRenderable.hpp"
#include "../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/QuadRenderable.hpp"

#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/Fence.hpp"

#include "../include/lighting/DirectionalLightRenderable.hpp"

#include "../include/MapRenderable.hpp"
#include "../include/BannerRenderable.hpp"


void initialize_practical_10_scene(Viewer& viewer)
{

    //Set up a shader and add a 3D frame.
    ShaderProgramPtr flatShader =
        std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                        "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);


    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setRestitution(0.0f);
    system->setDt(0.01);


    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep()in the animate() function
    //It also handles some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);



    glm::mat4 localTransformation(1.0);
    MaterialPtr normal = Material::Normal();

    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl", "../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    //Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -15, 15), glm::vec3(0,0,0), glm::vec3(0,0,1)) );

    viewer.addRenderable(std::make_shared<FrameRenderable>(flatShader));

    viewer.getCamera().setZfar(1000);

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,1.0), d_specular(0.5,0.5,0.5);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,0.0,5.0);
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.setDirectionalLight(directionalLight);
    viewer.addRenderable(directionalLightRenderable);

    std::string filename = "../textures/snow.jpg";
    std::shared_ptr<MapRenderable> map
        = std::make_shared<MapRenderable>(texShader, filename);
    map->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, 0.0)));
    map->setMaterial(normal);
    viewer.addRenderable(map);
    map->generateSapin(viewer, texShader);
    
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    //Initialize a plane at the bottom of the scene
    p1 = glm::vec3(400.0, 50.0, 0.0);
    p2 = glm::vec3(400.0, -50.0, 0.0);
    p3 = glm::vec3(450.0, 50.0, 0.0);
    PlanePtr planeBottom = std::make_shared<Plane>(p1, p2, p3);
    planeBottom->setGround(true);
    system->addPlaneObstacle(planeBottom);
    
    //Initialize a plane from 3 points and add it to the system as an obstacle
    p1 = glm::vec3(-400.0, 50.0, 400.0);
    p2 = glm::vec3(-400.0, -50.0, 400.0);
    p3 = glm::vec3(400.0, -50.0, 0.0);
    PlanePtr plane = std::make_shared<Plane>(p1, p2, p3);
    plane->setGround(true);
    system->addPlaneObstacle(plane);
    map->generateTremplin(plane, systemRenderable, texShader);
    
    //Initialize three planes for collision on left and right of the slope and at the end
    p1 = glm::vec3(0.0, 50.0, 0.0);
    p2 = glm::vec3(10.0, 50.0, 0.0);
    p3 = glm::vec3(0.0, 50.0, 10.0);
    PlanePtr planeLeft = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(planeLeft);
    p1[1] = -50.0;
    p2[1] = -50.0;
    p3[1] = -50.0;
    PlanePtr planeRight = std::make_shared<Plane>(p1, p3, p2);
    system->addPlaneObstacle(planeRight);
    p1 = glm::vec3(450.0, 50.0, 0.0);
    p2 = glm::vec3(450.0, -50.0, 0.0);
    p3 = glm::vec3(450.0, 50.0, 10.0);
    PlanePtr planeEnd = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(planeEnd);
    

    /*
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    glm::mat4 parentTransformation = glm::mat4(1.0);
    parentTransformation = glm::scale(parentTransformation, glm::vec3(800.0,800.0,1.0));
    parentTransformation = glm::translate(parentTransformation, glm::vec3(1.0,0.0,0.0));
    texPlane->setParentTransform(parentTransformation);
    texPlane->setMaterial(normal);
    viewer.addRenderable(texPlane);
    */

    //Création de la particule associée au skieur
    glm::vec3 px(-380.0, 0.0,400.0);
    glm::vec3 pv(0.0, 0.0, 0.0);
    float pm = 85.0, pr = 4.0;
    ParticleSkieurPtr mobile = std::make_shared<ParticleSkieur>( px, pv, pm, pr);
    system->addSkieur( mobile );

    //Skieur
    SkieurRenderablePtr skieur = std::make_shared<SkieurRenderable>(texShader, systemRenderable, mobile);
    HierarchicalRenderable::addChild(systemRenderable, skieur);
    skieur->initControlledSkieur(flatShader, systemRenderable);
    skieur->initForcesSkieur(system, systemRenderable, flatShader, mobile);
    viewer.getCamera().setParticle(mobile);

    //Drapeau 1
    std::shared_ptr<BannerRenderable> banner1 = std::make_shared<BannerRenderable>(flatShader);
    banner1->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(392.0, 48.0, 0)));
    viewer.addRenderable(banner1); 

    //Drapeau
    std::shared_ptr<BannerRenderable> banner2 = std::make_shared<BannerRenderable>(flatShader);
    banner2->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(392.0, -48.0, 0)));
    viewer.addRenderable(banner2); 
        
    /********** Fences ***********/
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(-380.0,48.0,387.0));
    glm::mat4 transfo(1.0);
    //Fences on the left and right
    transfo = glm::scale(transfo, glm::vec3(3.0,3.0,3.0));
    transfo = glm::rotate(transfo, 0.463f, glm::vec3(0.0,1.0,0.0));
    transfo = glm::rotate(transfo, 1.57079632679f, glm::vec3(0.0,1.0,0.0));
    transfo = glm::rotate(transfo, 1.57079632679f, glm::vec3(0.0,0.0,1.0));
    addFences(systemRenderable, texShader, 58, translate*transfo);
    translate = glm::translate(glm::mat4(1.0), glm::vec3(-380.0,-48.0,387.0));
    addFences(systemRenderable, texShader, 58, translate*transfo);
    //Fences at the bottom
    transfo = glm::scale(glm::mat4(1.0), glm::vec3(3.0,3.0,3.0));
    transfo = glm::rotate(transfo, 1.57079632679f, glm::vec3(0.0,1.0,0.0));
    transfo = glm::rotate(transfo, 1.57079632679f, glm::vec3(0.0,0.0,1.0));
    translate = glm::translate(glm::mat4(1.0), glm::vec3(401.0,48.0,-2.0));
    addFences(systemRenderable, texShader, 4, translate*transfo);
    translate = glm::translate(glm::mat4(1.0), glm::vec3(401.0,-48.0,-2.0));
    addFences(systemRenderable, texShader, 4, translate*transfo);
    //Fences at the end
    transfo = glm::scale(glm::mat4(1.0), glm::vec3(3.0,3.2,3.0));
    transfo = glm::rotate(transfo, 1.57079632679f, glm::vec3(1.0,0.0,0.0));
    translate = glm::translate(glm::mat4(1.0), glm::vec3(450.0,36.0,-2.0));
    addFences(systemRenderable, texShader, 6, translate*transfo);
    

	viewer.setAnimationLoop(false, 2*3.14159265);
    viewer.startAnimation();


}
