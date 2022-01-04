#pragma once

#include<iostream>
#include<fstream>
#include<string>

// GLM provides math functions
#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <GL/glew.h> // provides newer OpenGL functionality
#include <GL/freeglut.h> // create and manage windows containing OpenGL contexts
#include "SOIL.h" // to load images


#include "loadShaders.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
	glm::vec3 normal;
};