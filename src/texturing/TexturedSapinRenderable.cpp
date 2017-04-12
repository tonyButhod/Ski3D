#include "../../include/texturing/TexturedSapinRenderable.hpp"
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
#include <time.h>

TexturedSapinRenderable::TexturedSapinRenderable(
        ShaderProgramPtr shaderProgram, const std::string& textureFilename)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_nBuffer(0), m_tBuffer(0), m_texId(0),
      m_wrapOption(0), m_filterOption(0)
{
    //Ajout du tronc du sapin
    std::vector<glm::vec3> tmp_x, tmp_n;
    std::vector<glm::vec2> tmp_tex;
    unsigned int strips=10, slices=10;
    glm::mat4 transformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,10.0));
    
    getUnitCone(tmp_x, tmp_n, tmp_tex, strips, slices);
    for(size_t i=0; i<tmp_x.size(); ++i) {
        m_positions.push_back(glm::vec3(transformation*glm::vec4(tmp_x[i],1.0)));
        m_normals.push_back(normalize(glm::vec3(transformation*glm::vec4(tmp_n[i],1.0))));
        m_origTexCoords.push_back(tmp_tex[i]);
        m_texCoords.push_back(tmp_tex[i]);
    }
    //Ajout des branches du sapin
    srand(time(NULL));
    double start = 2.5;
    double nb_branches = 30;
    double pas = (10.0-start)/nb_branches;
    float aleat = 0.0f;
    for (int i=0; i<nb_branches-5; ++i) {
        transformation = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, start+pas*i));
        aleat += (float)((rand()%3141) +1570)/1000.0f;
        transformation = glm::rotate(transformation, aleat, glm::vec3(0.0, 0.0, 1.0));
        transformation = glm::rotate(transformation, 1.57079632679f, glm::vec3(0.0, 1.0, 0.0));
        double base = 0.2*(1.0-i/nb_branches);
        transformation = glm::scale(transformation, glm::vec3(base,base,(double)(nb_branches-i)*5.0/nb_branches));
        
        for(size_t i=0; i<tmp_x.size(); ++i) {
            m_positions.push_back(glm::vec3(transformation*glm::vec4(tmp_x[i],1.0)));
            m_normals.push_back(normalize(glm::vec3(transformation*glm::vec4(tmp_n[i],1.0))));
            m_origTexCoords.push_back(tmp_tex[i]);
            m_texCoords.push_back(tmp_tex[i]);
        }
        //On ajoute des feuilles de chaque côté
        create_leaf(transformation);
        
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
    image.loadFromFile(textureFilename);
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

void TexturedSapinRenderable::create_leaf(glm::mat4 &parentTransfo) {
    glm::mat4 transfo = glm::rotate(parentTransfo, 1.57079632679f, glm::vec3(0.0,0.0,1.0));
    int precision = 5;
    double x_branche = 0.15;
    double init_width = 7.0;
    double init_height = -6.0;
    //On trace un côté de la branche
    double pas = (1.0 - x_branche)/precision;
    double ampli_y = 0.5*(double)((rand()%400)+900.0)/1000.0;
    for (int i=0; i<precision; i++) {
        m_positions.push_back(glm::vec3(transfo*glm::vec4(0,0,x_branche+pas*i,1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,0.0));
        m_texCoords.push_back(glm::vec2(0.5,0.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(-init_width*ampli_y,init_height*ampli_y,x_branche+pas*i,1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(1.0,0.5));
        m_texCoords.push_back(glm::vec2(1.0,0.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(0,0,x_branche+pas*(i+1),1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,1.0));
        m_texCoords.push_back(glm::vec2(0.5,1.0));
        //Deuxième triangle
        double new_ampli = (double)(precision-i-1)/precision*((rand()%400)+900.0)/1000.0;
        m_positions.push_back(glm::vec3(transfo*glm::vec4(0,0,x_branche+pas*(i+1),1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,0.0));
        m_texCoords.push_back(glm::vec2(0.5,1.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(-init_width*new_ampli,init_height*new_ampli,x_branche+pas*(i+1),1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(1.0,0.5));
        m_texCoords.push_back(glm::vec2(1.0,1.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(-init_width*ampli_y,init_height*ampli_y,x_branche+pas*i,1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,1.0));
        m_texCoords.push_back(glm::vec2(1.0,0.0));
        ampli_y = new_ampli;
    }
    //On trace un côté de la branche
    double pas_x = (1.0 - x_branche)/precision;
    ampli_y = 0.5*(double)((rand()%400)+900.0)/1000.0;
    for (int i=0; i<precision; i++) {
        m_positions.push_back(glm::vec3(transfo*glm::vec4(0,0,x_branche+pas*i,1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,0.0));
        m_texCoords.push_back(glm::vec2(0.5,0.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(init_width*ampli_y,init_height*ampli_y,x_branche+pas*i,1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(1.0,0.5));
        m_texCoords.push_back(glm::vec2(1.0,0.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(0,0,x_branche+pas*(i+1),1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,1.0));
        m_texCoords.push_back(glm::vec2(0.5,1.0));
        //Deuxième triangle
        double new_ampli = (double)(precision-i-1)/precision*((rand()%400)+900.0)/1000.0;
        m_positions.push_back(glm::vec3(transfo*glm::vec4(0,0,x_branche+pas*(i+1),1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,0.0));
        m_texCoords.push_back(glm::vec2(0.5,1.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(init_width*new_ampli,init_height*new_ampli,x_branche+pas*(i+1),1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(1.0,0.5));
        m_texCoords.push_back(glm::vec2(1.0,1.0));
        m_positions.push_back(glm::vec3(transfo*glm::vec4(init_width*ampli_y,init_height*ampli_y,x_branche+pas*i,1.0)));
        m_normals.push_back(normalize(glm::vec3(transfo*glm::vec4(0,1,0,1.0))));
        m_origTexCoords.push_back(glm::vec2(0.5,1.0));
        m_texCoords.push_back(glm::vec2(1.0,0.0));
        ampli_y = new_ampli;
    }
}

TexturedSapinRenderable::~TexturedSapinRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));

    glcheck(glDeleteTextures(1, &m_texId));
}

void TexturedSapinRenderable::do_draw()
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

void TexturedSapinRenderable::do_animate(float time)
{
}

void TexturedSapinRenderable::do_keyPressedEvent( sf::Event& e )
{
}

void TexturedSapinRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

void getUnitCone(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords, 
        const unsigned int& strips, const unsigned int& slices)
{
    double height = 1.0;
    double radius=1.0;
    glm::vec3 center(0.0,0.0,0.0);
    int numberOfVertices = slices*strips*6 + slices*3;
    positions.resize(numberOfVertices);
    normals.resize(numberOfVertices);
    texCoords.resize(numberOfVertices);

    //Lateral surface
    for(int i=0; i<slices; ++i)
    {
        for(int j=0; j<strips-1; ++j)
        {
            double curr_theta = i*(2.0*M_PI/(double)slices);
            double curr_u = j*(height/(double)strips);
            double next_theta = (i+1)*(2.0*M_PI/(double)slices);
            double next_u = (j+1)*(height/(double)strips);
            double factor1 = (height-curr_u)/height;
            double factor2 = (height-next_u)/height;
            if (j == strips-2)
                factor2 = 0;
            double next_tex = (double)(i%(slices/2)+1)/slices;
            double curr_tex = (double)(i%(slices/2))/slices;

            positions[i*strips*6 + 6*j +0] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +1] = center + glm::vec3(factor1*radius*cos(next_theta),
                    factor1*radius*sin(next_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +2] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);

            normals[i*strips*6 + 6*j +0] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +1] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +2] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        next_u));
            
            texCoords[i*strips*6 + 6*j +0] = glm::vec2(curr_tex, 0.0);
            texCoords[i*strips*6 + 6*j +1] = glm::vec2(next_tex, 0.0);
            texCoords[i*strips*6 + 6*j +2] = glm::vec2(next_tex, 1.0);

            positions[i*strips*6 + 6*j +3] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +4] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);
            positions[i*strips*6 + 6*j +5] = center + glm::vec3(factor2*radius*cos(curr_theta),
                    factor2*radius*sin(curr_theta),
                    next_u);

            normals[i*strips*6 + 6*j +3] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +4] = glm::normalize(glm::vec3(cos(next_theta),
                        radius*sin(next_theta),
                        next_u));
            normals[i*strips*6 + 6*j +5] = glm::normalize(glm::vec3(cos(curr_theta),
                        radius*sin(curr_theta),
                        next_u));
            
            texCoords[i*strips*6 + 6*j +3] = glm::vec2(curr_tex, 0.0);
            texCoords[i*strips*6 + 6*j +4] = glm::vec2(next_tex, 1.0);
            texCoords[i*strips*6 + 6*j +5] = glm::vec2(curr_tex, 1.0);

        }
    }

    //Bottom faces
    unsigned int counter = slices*strips*6;
    for(int i=0; i<slices; ++i)
    {
        double curr_theta = i*(2.0*M_PI/(double)slices);
        double next_theta = (i+1)*(2.0*M_PI/(double)slices);
        double u_base = 0.0;

        positions[counter+3*i+0] = center + glm::vec3(0, 0, u_base );
        positions[counter+3*i+1] = center + glm::vec3(radius*cos(curr_theta), radius*sin(curr_theta), u_base);
        positions[counter+3*i+2] = center + glm::vec3(radius*cos(next_theta), radius*sin(next_theta), u_base);

        normals[counter+3*i+0] = glm::vec3(0, 0, -1);
        normals[counter+3*i+1] = glm::vec3(0, 0, -1);
        normals[counter+3*i+2] = glm::vec3(0, 0, -1);
    }

}