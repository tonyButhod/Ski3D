#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"

#include "../include/BannerRenderable.hpp"
#include "../include/FrameRenderable.hpp"
#include "../teachers/CylinderRenderable.hpp"
#include "../teachers/MeshRenderable.hpp"


void initialize_practical_02_scene(Viewer& viewer)
{
    // create all shaders of this scene, then add them to the viewer
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                          "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);


    // create renderable objects
    viewer.addRenderable(std::make_shared<FrameRenderable>(flatShader));

	//test
    std::shared_ptr<BannerRenderable> Pole
        = std::make_shared<BannerRenderable>(flatShader);
    Pole->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(-1.5, 0.0, 0.0)));
    viewer.addRenderable(Pole);
	
	viewer.setAnimationLoop(true, 2*3.14159265/8);
	viewer.startAnimation();

}
