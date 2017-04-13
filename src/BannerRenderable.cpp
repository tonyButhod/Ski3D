#include "../include/BannerRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

#define B_LENGTH 1
#define B_WIDTH 1
#define LENGTH_SUBDIVISIONS 10
#define WIDTH_SUBDIVISIONS 10


	BannerRenderable::BannerRenderable(ShaderProgramPtr shaderProgram)
: Renderable(shaderProgram),
	m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), start_index(0), m_time(0.0)
{
	float theta = 0.0;
	float h = 0.0;
	float x = 0.0, y = 0.0;
	int a = 20;	
	// Processing the color of the cylinder
	glm::vec4 color(0.5,0.5,0.5,0.5);

	int size_p = a*6 + a*3 + a*3 + LENGTH_SUBDIVISIONS * WIDTH_SUBDIVISIONS * 6;
	m_positions.resize(size_p, glm::vec3(0.0,0.0,0.0));

	int index = 0;

	for (int i = 0; i < a; ++i) {
		theta = i*2*PI/a;
		// Each facet is made of 2 triangles
		// First triangle
		x = 0.5 * cos(theta);
		y = 0.5 * sin(theta);
		m_positions[index] = (glm::vec3(x, y, 0.0));
		m_positions[index+1] = (glm::vec3(x, y, 25.0));
		x = 0.5 * cos(theta + 2*PI/a);
		y = 0.5 * sin(theta + 2*PI/a);
		m_positions[index+2] = (glm::vec3(x, y, 0.0));
		// Second triangle
		m_positions[index+3] = (glm::vec3(0.5 * cos(theta), 0.5 * sin(theta), 25.0));
		m_positions[index+4] = (glm::vec3(x, y, 25.0));
		m_positions[index+5] = (glm::vec3(x, y, 0.0));
		for (int j = 0; j < 6; ++j) {
			m_colors.push_back(color);
		}
		index += 6;
	}


	// Computing the bottom surface
	for (int i = 0; i < a; ++i) {
		theta = i*2*PI/a;
		x = 0.5 * cos(theta);
		y = 0.5 * sin(theta);
		m_positions[index] = (glm::vec3(x, y, 0.0));
		m_positions[index+1] = (glm::vec3(0.0, 0.0, 0.0));
		x = 0.5 * cos(theta + 2*PI/a);
		y = 0.5 * sin(theta + 2*PI/a);
		m_positions[index+2] = (glm::vec3(x, y, 0.0));
		for (int j = 0; j < 3; ++j) {
			m_colors.push_back(color);
		}
		index += 3;
	}


	// Computing the top surface
	for (int i = 0; i < a; ++i) {
		theta = i*2*PI/a;
		x = 0.5 * cos(theta);
		y = 0.5 * sin(theta);
		m_positions[index] = (glm::vec3(x, y, 25.0));
		m_positions[index+1] = (glm::vec3(0.0, 0.0, 25.0));
		x = 0.5 * cos(theta + 2*PI/a);
		y = 0.5 * sin(theta + 2*PI/a);
		m_positions[index+2] = (glm::vec3(x, y, 25.0));
		for (int j = 0; j < 3; ++j) {
			m_colors.push_back(color);
		}
		index += 3;
	}


	// Computing the flag
	x = 0.5;
	y = 0.0;
	float z = 0.0;
	float dl = (float) LENGTH / LENGTH_SUBDIVISIONS;
	float dw = (float) WIDTH / WIDTH_SUBDIVISIONS;

	/* m_positions.push_back(glm::vec3(x, y, z));
	   m_positions.push_back(glm::vec3(x + dl, y, z));
	   m_positions.push_back(glm::vec3(x + dl, y, z - dw));
	   for (int k=0; k < 3; ++k) {
	   m_colors.push_back(color);
	   } 
	   m_positions.push_back(glm::vec3(x, y, z));
	   m_positions.push_back(glm::vec3(x + dl, y, z - dw));
	   m_positions.push_back(glm::vec3(x , y, z - dw));
	   for (int k=0; k < 3; ++k) {
	   m_colors.push_back(color);
	   } */

	start_index = index;

	glm::vec4 blanc(1.0, 1.0, 1.0, 1.0);
	glm::vec4 noir(0.0,0.0,0.0,1.0);
	color = noir;
	int couleur = 0;

	for (int i=0; i < LENGTH_SUBDIVISIONS; ++i) {
		for (int j=0; j < WIDTH_SUBDIVISIONS; ++j) {
			if (j%2 == couleur) {
				color = noir;
			} else {
				color = blanc;
			}
			m_positions[index] = (glm::vec3(x + i*dl,
					   	y + sin(PI*i*dl/LENGTH) + sin(2*PI*i*dl/LENGTH), z + j*dw ));
			m_positions[index+1] = (glm::vec3(x + (i+1)*dl,
					   	y + sin(PI*(i+1)*dl/LENGTH) + sin(2*PI*(i+1)*dl/LENGTH), z + j*dw));
			m_positions[index+2] = (glm::vec3(x + (i+1)*dl, 
						y + sin(PI*(i+1)*dl/LENGTH) + sin(2*PI*(i+1)*dl/LENGTH), z + (j+1)*dw));
			for (int k=0; k < 3; ++k) {
				m_colors.push_back(color);
			}

			m_positions[index+3] = (glm::vec3(x + i*dl, 
						y + sin(PI*i*dl/LENGTH) + sin(2*PI*i*dl/LENGTH), z + j*dw));
			m_positions[index+4] = (glm::vec3(x + (i+1)*dl, 
						y + sin(PI*(i+1)*dl/LENGTH) + sin(2*PI*i*dl/LENGTH), z + (j+1)*dw));
			m_positions[index+5] = (glm::vec3(x + i*dl, 
						y + sin(PI*i*dl/LENGTH) + sin(2*PI*i*dl/LENGTH), z + (j+1)*dw));
			for (int k=0; k < 3; ++k) {
				m_colors.push_back(color);
			}
			index += 6;
		}
		couleur = 1 - couleur;
	}

	// Step 2
	// Vertices
	glGenBuffers(1, &m_pBuffer);
	// Colors
	glGenBuffers(1, &m_cBuffer);
	// Normals
	glGenBuffers(1, &m_nBuffer);

	// Step 3
	glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW);

}

BannerRenderable::~BannerRenderable()
{
	glcheck(glDeleteBuffers(1, &m_pBuffer));
	glcheck(glDeleteBuffers(1, &m_cBuffer));
	glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void BannerRenderable::do_draw()
{
	//Location
	int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
	int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
	int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
	int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

	//Send data to GPU
	if (modelLocation != ShaderProgram::null_location) {
		glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
	}

	if (positionLocation != ShaderProgram::null_location) {
		//Activate location
		glcheck(glEnableVertexAttribArray(positionLocation));
		//Bind buffer
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
		//Specify internal format
		glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	}
	if (colorLocation != ShaderProgram::null_location) {
		glcheck(glEnableVertexAttribArray(colorLocation));
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
		glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
	}
	if (normalLocation != ShaderProgram::null_location) {
		glcheck(glEnableVertexAttribArray(normalLocation));
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
		glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	}

	glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

	if (positionLocation != ShaderProgram::null_location) {
		glcheck(glDisableVertexAttribArray(positionLocation));
	}
	if (colorLocation != ShaderProgram::null_location) {
		glcheck(glDisableVertexAttribArray(colorLocation));
	}
	if (normalLocation != ShaderProgram::null_location) {
		glcheck(glDisableVertexAttribArray(normalLocation));
	}
}

void BannerRenderable::do_animate(float time)
{
	// Computing the flag
	float x = 0.5;
	float y = 0.0;
	float z = 0.0;
	float dl = (float) LENGTH / LENGTH_SUBDIVISIONS;
	float dw = (float) WIDTH / WIDTH_SUBDIVISIONS;
	glm::vec4 color = randomColor();
	int index = start_index;
	float v = sqrt(108/1.83);

	float w = 2*PI*0.5/LENGTH;
	float k = 2*PI / LENGTH;

	float t = time + PI/LENGTH;

	if (t == 0) {
		t = m_time;
		m_time = time;
	}

	for (int i=0; i < LENGTH_SUBDIVISIONS; ++i) {
		for (int j=0; j < WIDTH_SUBDIVISIONS; ++j) {
			m_positions[index] = (glm::vec3(x + i*dl, 
						y + 0*sin(PI*i*dl/LENGTH)*cos(PI*v*t/LENGTH) + (i*dl)*cos(2*PI*(i*dl-v*t)/LENGTH)/LENGTH, z + j*dw ));
			m_positions[index+1] = (glm::vec3(x + (i+1)*dl, 
						y + 0*sin(PI*(i+1)*dl/LENGTH)*cos(PI*v*t/LENGTH) + (i+1)*dl*cos(2*PI*((i+1)*dl-v*t)/LENGTH)/LENGTH, z + j*dw));
			m_positions[index+2] = (glm::vec3(x + (i+1)*dl, 
						y + 0*sin(PI*(i+1)*dl/LENGTH)*cos(PI*v*t/LENGTH) + (i+1)*dl*cos(2*PI*((i+1)*dl-v*t)/LENGTH)/LENGTH, z + (j+1)*dw));

			m_positions[index+3] = (glm::vec3(x + i*dl, 
					y + 0*sin(PI*i*dl/LENGTH)*cos(PI*v*t/LENGTH) + i*dl*cos(2*PI*(i*dl-v*t)/LENGTH)/LENGTH, z + j*dw));
			m_positions[index+4] = (glm::vec3(x + (i+1)*dl, 
					y + 0*sin(PI*(i+1)*dl/LENGTH)*cos(PI*v*t/LENGTH) + (i+1)*dl*cos(2*PI*((i+1)*dl-v*t)/LENGTH)/LENGTH, z + (j+1)*dw));
			m_positions[index+5] = (glm::vec3(x + i*dl, 
					y + 0*sin(PI*i*dl/LENGTH)*cos(PI*v*t/LENGTH) + i*dl*cos(2*PI*(i*dl-v*t)/LENGTH)/LENGTH, z + (j+1)*dw));
			index += 6;
		}
	}

	// Step 3
	glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);

}
