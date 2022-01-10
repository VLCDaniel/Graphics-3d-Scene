#pragma once

// Standard Libraries
#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <minmax.h>
#include <sstream>

// GLM provides math functions
#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <GL/glew.h> // provides newer OpenGL functionality
#include <GL/freeglut.h> // create and manage windows containing OpenGL contexts


// Local Libraries
#include "SOIL.h" // to load images
#include "Camera.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Primitive.h"
#include "Mesh.h"
#include "Model.h"
#include "OBJLoader.h"