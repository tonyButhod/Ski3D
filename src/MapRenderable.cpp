#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <math.h>

#include "./../include/MapRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include "../include/Viewer.hpp"

#include "../include/noisegen.h"
#include "../teachers/MeshRenderable.hpp"

#include "../include/texturing/TexturedSapinRenderable.hpp"



#define MIN_X   -400
#define MIN_Y   -400
#define MAX_X   400
#define MAX_Y   400

#define MMIN_X   -50
#define MMIN_Y   -400
#define MMAX_X   50
#define MMAX_Y   400

#define NMIN_X   -200
#define NMAX_X   200

#define STEP_X  4
#define STEP_Y  20

#define SEED        51
#define MAX_HEIGHT  100
#define FREQUENCE   100

#define RARETE_SAPIN  500
#define MAX_SAPIN     400




int getOffset(int y)
{
    return (int)(200+(float)-0.5*(float)y);
}


int getNoise(int y, int x)
{
    double height;

    //ValueCoherentNoise3D

    if(x>=MMIN_X && x<=MMAX_X && y>=MMIN_Y && y<=MMAX_Y)
        height = getOffset(y);
    else if(x>=NMIN_X && x<=MMIN_X)
        height = getOffset(y) + noise::ValueCoherentNoise3D(((float)x)/FREQUENCE, ((float)y)/FREQUENCE, SEED) * MAX_HEIGHT * sqrt((float)abs(x-MMIN_X)/((float)abs(NMIN_X-MMIN_X)));
    else if(x<=NMAX_X && x>=MMAX_X)
        height = getOffset(y) + noise::ValueCoherentNoise3D(((float)x)/FREQUENCE, ((float)y)/FREQUENCE, SEED) * MAX_HEIGHT * sqrt((float)abs(x-MMAX_X)/((float)abs(NMAX_X-MMAX_X)));
    else
        height = getOffset(y) + noise::ValueCoherentNoise3D(((float)x)/FREQUENCE, ((float)y)/FREQUENCE, SEED) * MAX_HEIGHT;


    return (int)height;
}



glm::vec3 computeNormal(glm::vec3 const & a, glm::vec3 const & b, glm::vec3 const & c)
{
    return glm::normalize(glm::cross(c - a, b - a));
}


glm::vec3 computeMeanNormal(int i, int j)
{
      glm::vec3 a = glm::vec3(i, j, getNoise(i, j));
      glm::vec3 b = glm::vec3(i, j+STEP_X, getNoise(i, j+STEP_X));
      glm::vec3 c = glm::vec3(i+STEP_X,  j, getNoise(i+STEP_X,  j));
      glm::vec3 n1 = computeNormal(a, b, c);

      glm::vec3 n2;
      if(i > MIN_Y)
      {
          a = glm::vec3(i-STEP_X, j, getNoise(i-STEP_X, j));
          b = glm::vec3(i-STEP_X, j+STEP_X, getNoise(i-STEP_X, j+STEP_X));
          c = glm::vec3(i,  j, getNoise(i,  j));
          n2 = computeNormal(a, b, c);
      }
      else
          n2 = glm::vec3(0.0,0.0,0.0);

      glm::vec3 n3;
      if(i+STEP_X<MAX_Y)
      {
          a = glm::vec3(i+STEP_X, j, getNoise(i+STEP_X, j));
          b = glm::vec3(i+STEP_X, j+STEP_X, getNoise(i+STEP_X, j+STEP_X));
          c = glm::vec3(i+STEP_X+STEP_X,  j, getNoise(i+STEP_X+STEP_X,  j));
          n3 = computeNormal(a, b, c);
      }
      else
          n3 = glm::vec3(0.0,0.0,0.0);

      glm::vec3 n4;
      if(j > MIN_X)
      {
          a = glm::vec3(i, j-STEP_X, getNoise(i, j-STEP_X));
          b = glm::vec3(i, j, getNoise(i, j));
          c = glm::vec3(i+STEP_X,  j-STEP_X, getNoise(i+STEP_X,  j-STEP_X));
          n4 = computeNormal(a, b, c);
      }
      else
          n4 = glm::vec3(0.0,0.0,0.0);

      glm::vec3 n5;
      if(j+STEP_X<MAX_X)
      {
          a = glm::vec3(i, j+STEP_X, getNoise(i, j+STEP_X));
          b = glm::vec3(i, j+STEP_X+STEP_X, getNoise(i, j+STEP_X+STEP_X));
          c = glm::vec3(i+STEP_X,  j+STEP_X, getNoise(i+STEP_X,  j+STEP_X));
          n5 = computeNormal(a, b, c);
      }
      else
          n5 = glm::vec3(0.0,0.0,0.0);

      return n1+n2+n3+n4+n5;
}



MapRenderable::MapRenderable(ShaderProgramPtr shaderProgram, const std::string& textureFilename) :
    Renderable(shaderProgram),
    m_pBuffer(0), m_nBuffer(0), m_tBuffer(0), m_texId(0)
{
    //Build the geometry
    int i, j;

    for(i=MIN_X; i<MAX_X; i += STEP_X)
    {
        for(j=MIN_Y; j<MAX_Y; j += STEP_X)
        {
            glm::vec3 n;

            glm::vec3 a = glm::vec3(i, j, getNoise(i, j));
            glm::vec3 b = glm::vec3(i, j+STEP_X, getNoise(i, j+STEP_X));
            glm::vec3 c = glm::vec3(i+STEP_X,  j, getNoise(i+STEP_X,  j));

            m_positions.push_back(a);
            m_positions.push_back(b);
            m_positions.push_back(c);

            n = computeMeanNormal(i,j);
            m_normals.push_back(n);
            n = computeMeanNormal(i,j+STEP_X);
            m_normals.push_back(n);
            n = computeMeanNormal(i+STEP_X,j);
            m_normals.push_back(n);


            a = glm::vec3(i, j+STEP_X, getNoise(i, j+STEP_X));
            b = glm::vec3(i+STEP_X,  j, getNoise(i+STEP_X,  j));
            c = glm::vec3(i+STEP_X,  j+STEP_X, getNoise(i+STEP_X,  j+STEP_X));

            //n = computeNormal(c, b, a);

            m_positions.push_back(a);
            m_positions.push_back(b);
            m_positions.push_back(c);

            n = computeMeanNormal(i,j+STEP_X);
            m_normals.push_back(n);
            n = computeMeanNormal(i+STEP_X,j);
            m_normals.push_back(n);
            n = computeMeanNormal(i+STEP_X,j+STEP_X);
            m_normals.push_back(n);


            m_texCoords.push_back(glm::vec2(0.0,0.0));
            m_texCoords.push_back(glm::vec2(1.0,0.0));
            m_texCoords.push_back(glm::vec2(0.0,1.0));

            m_texCoords.push_back(glm::vec2(0.0,1.0));
            m_texCoords.push_back(glm::vec2(1.0,0.0));
            m_texCoords.push_back(glm::vec2(1.0,1.0));
        }
    }

    //Set the model matrix to identity
    m_model = glm::mat4(1.0);

    glGenBuffers(1, &m_pBuffer);
    glcheck(glGenBuffers(1, &m_nBuffer));
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));

    // === PART 2: Texture
    glGenBuffers(1, &m_tBuffer); //texture coords
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

    sf::Image image;
    image.loadFromFile(textureFilename);
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


MapRenderable::~MapRenderable()
{
    glDeleteBuffers(1, &m_pBuffer);
    glDeleteBuffers(1, &m_nBuffer);
    glDeleteBuffers(1, &m_tBuffer);
    glcheck(glDeleteTextures(1, &m_texId));
}


void MapRenderable::do_draw()
{

    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model));

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texSamplerLocation = m_shaderProgram->getUniformLocation("texSampler");
    int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

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

    glEnableVertexAttribArray(positionLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glcheck(glEnableVertexAttribArray(normalLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

    // Texture and texture coordinates
    if (texCoordLocation != ShaderProgram::null_location) {
        // Bind texture on texture unit 0
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocation, 0));

        // Send texture coordinates attributes
        glcheck(glEnableVertexAttribArray(texCoordLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw the triangles
    glDrawArrays(GL_TRIANGLES,0, m_positions.size());

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

void MapRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

void MapRenderable::do_animate(float time)
{
}



// Fonction permettant de générer des sapins sur la map, une fois que celle-ci a été générée
void MapRenderable::generateSapin(Viewer& viewer, ShaderProgramPtr texShader)
{
    int i, j;

    int nbSapin = 0;

    srand(time(NULL));

    std::string filename = "../textures/sapin_snow.png";
    MaterialPtr normalMat = Material::Normal();
    glm::mat4 localTransformation;

    for(j=NMIN_X; j<MMIN_X; j += STEP_X)
    {
        for(i=MIN_Y; i<MAX_Y; i += STEP_X)
        {
            int r = rand() % RARETE_SAPIN;

            if(r == 0 && nbSapin < MAX_SAPIN)
            {
                // On genere le sapin dans la zone
                TexturedSapinRenderablePtr texSapin = std::make_shared<TexturedSapinRenderable>(texShader, filename);
                texSapin->setMaterial(normalMat);
                localTransformation = glm::translate(glm::mat4(1.0), glm::vec3(i,j,getNoise(i, j)));
                localTransformation = glm::scale(localTransformation, glm::vec3(3.0,3.0,3.0));
                texSapin->setLocalTransform(localTransformation);
                viewer.addRenderable(texSapin);
                nbSapin++;
            }
        }
    }

    for(j=MMAX_X; j<NMAX_X; j += STEP_X)
    {
        for(i=MIN_Y; i<MAX_Y; i += STEP_X)
        {
            int r = rand() % RARETE_SAPIN;

            if(r == 0 && nbSapin < MAX_SAPIN)
            {
                // On genere le sapin dans la zone
                TexturedSapinRenderablePtr texSapin = std::make_shared<TexturedSapinRenderable>(texShader, filename);
                texSapin->setMaterial(normalMat);
                localTransformation = glm::translate(glm::mat4(1.0), glm::vec3(i,j,getNoise(i, j)));
                localTransformation = glm::scale(localTransformation, glm::vec3(3.0,3.0,3.0));
                texSapin->setLocalTransform(localTransformation);
                viewer.addRenderable(texSapin);
                nbSapin++;
            }
        }
    }
}
