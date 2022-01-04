#pragma once

#include <iostream>


#include "glm/glm/glm.hpp"
#include <GL/glew.h> // provides newer OpenGL functionality
#include <GL/freeglut.h> // create and manage windows containing OpenGL contexts
#include "SOIL.h" // to load images


#include "loadShaders.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
};