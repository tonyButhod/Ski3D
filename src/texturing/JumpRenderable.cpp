#include "../../include/texturing/JumpRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../teachers/Geometries.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

JumpRenderable::JumpRenderable(
        ShaderProgramPtr shaderProgram)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_nBuffer(0), m_tBuffer(0), m_texId(0),
      m_wrapOption(0), m_filterOption(0),
      m_cornerMin(0.0), m_cornerMax(0.0), m_n(0.0,0.0,1.0)
{
    //Ajout d'un saut
    std::vector<glm::vec3> tmp_x, tmp_n;
    std::vector<glm::vec2> tmp_tex;
    constructJump(tmp_x, tmp_n, tmp_tex);
    for(size_t i=0; i<tmp_x.size(); ++i) {
        m_positions.push_back(glm::vec3(glm::vec4(tmp_x[i],1.0)));
        m_normals.push_back(normalize(glm::vec3(glm::vec4(tmp_n[i],1.0))));
        m_origTexCoords.push_back(tmp_tex[i]);
        m_texCoords.push_back(tmp_tex[i]);
    }
    
    // === PART 1: Vertex attributes, except texture coordinates
    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));

    // === PART 2: Texture
    // texture coordinates: just like any other vertex attributes!
    glGenBuffers(1, &m_tBuffer); //texture coords
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

    // now handle the "texture image" itself
    // load the image (here using the sfml library)
    sf::Image image;
    image.loadFromFile("../textures/grass_texture.png");
    // sfml inverts the v axis...
    // Hence, flip it to put the image in OpenGL convention: lower left corner is (0,0)
    image.flipVertically();

    // create a GPU buffer then bind the texture
    glcheck(glGenTextures(1, &m_texId));
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

    // texture options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Transfer the texture image texture to the GPU
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
        image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        (const GLvoid*)image.getPixelsPtr()));

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));
}

JumpRenderable::~JumpRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));

    glcheck(glDeleteTextures(1, &m_texId));
}

void JumpRenderable::do_draw()
{    
    //Locations
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texSamplerLocaction = m_shaderProgram->getUniformLocation("texSampler");

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");

    //Send material as uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send uniforms to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }
    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }

    //Send vertex attributes to GPU
    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    // Texture and texture coordinates
    if (texCoordLocation != ShaderProgram::null_location) {
        // Bind texture on texture unit 0
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocaction, 0));

        // Send texture coordinates attributes
        glcheck(glEnableVertexAttribArray(texCoordLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    //Release everything
    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }

    if (texCoordLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(texCoordLocation));
        glcheck(glBindTexture(GL_TEXTURE_2D, 0));   // unbind the texture!
    }
}

void JumpRenderable::do_animate(float time)
{
}

void JumpRenderable::do_keyPressedEvent( sf::Event& e )
{
}

void JumpRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}


void constructJump(std::vector<glm::vec3> &pos, std::vector<glm::vec3> &norm, 
        std::vector<glm::vec2> &tex) {
    /********* Positions *********/
    //Dessus du saut
    pos.push_back(glm::vec3(-1,-1,0));
    pos.push_back(glm::vec3(1,-1,0));
    pos.push_back(glm::vec3(-1,1,1));
    pos.push_back(glm::vec3(-1,1,1));
    pos.push_back(glm::vec3(1,-1,0));
    pos.push_back(glm::vec3(1,1,1));
    //Derriere du saut
    pos.push_back(glm::vec3(-1,1,0));
    pos.push_back(glm::vec3(1,1,0));
    pos.push_back(glm::vec3(-1,1,1));
    pos.push_back(glm::vec3(-1,1,1));
    pos.push_back(glm::vec3(1,1,0));
    pos.push_back(glm::vec3(1,1,1));
    //Cotés latéraux du saut
    pos.push_back(glm::vec3(-1,-1,0));
    pos.push_back(glm::vec3(-1,1,0));
    pos.push_back(glm::vec3(-1,1,1));
    
    pos.push_back(glm::vec3(1,-1,0));
    pos.push_back(glm::vec3(1,1,0));
    pos.push_back(glm::vec3(1,1,1));
    
    /******** Normales *********/
    //Dessus du saut
    norm.push_back(glm::vec3(0,-1,1));
    norm.push_back(glm::vec3(0,-1,1));
    norm.push_back(glm::vec3(0,-1,1));
    norm.push_back(glm::vec3(0,-1,1));
    norm.push_back(glm::vec3(0,-1,1));
    norm.push_back(glm::vec3(0,-1,1));
    //Derriere du saut
    norm.push_back(glm::vec3(0,1,0));
    norm.push_back(glm::vec3(0,1,0));
    norm.push_back(glm::vec3(0,1,0));
    norm.push_back(glm::vec3(0,1,0));
    norm.push_back(glm::vec3(0,1,0));
    norm.push_back(glm::vec3(0,1,0));
    //Cotés latéraux du saut
    norm.push_back(glm::vec3(-1,0,0));
    norm.push_back(glm::vec3(-1,0,0));
    norm.push_back(glm::vec3(-1,0,0));
    
    norm.push_back(glm::vec3(1,0,0));
    norm.push_back(glm::vec3(1,0,0));
    norm.push_back(glm::vec3(1,0,0));
    
    /******** Texture *********/
    //Dessus du saut
    tex.push_back(glm::vec2(0,0));
    tex.push_back(glm::vec2(1,0));
    tex.push_back(glm::vec2(0,1));
    tex.push_back(glm::vec2(0,1));
    tex.push_back(glm::vec2(1,0));
    tex.push_back(glm::vec2(1,1));
    //Derriere du saut
    tex.push_back(glm::vec2(0,0));
    tex.push_back(glm::vec2(1,0));
    tex.push_back(glm::vec2(0,1));
    tex.push_back(glm::vec2(0,1));
    tex.push_back(glm::vec2(1,0));
    tex.push_back(glm::vec2(1,1));
    //Cotés latéraux du saut
    tex.push_back(glm::vec2(0,0));
    tex.push_back(glm::vec2(1,0));
    tex.push_back(glm::vec2(1,1));
    
    tex.push_back(glm::vec2(0,0));
    tex.push_back(glm::vec2(1,0));
    tex.push_back(glm::vec2(1,1));
}

void JumpRenderable::setCornerMin(glm::vec3 &corner) {
    m_cornerMin = corner;
}

void JumpRenderable::setCornerMax(glm::vec3 &corner) {
    m_cornerMax = corner;
}

void JumpRenderable::setNormal(glm::vec3 &normal) {
    m_n = normal;
}

const glm::vec3 & JumpRenderable::getCornerMax() {
    return m_cornerMax;
}

const glm::vec3 & JumpRenderable::getCornerMin() {
    return m_cornerMin;
}

const glm::vec3 & JumpRenderable::getNormal() {
    return m_n;
}