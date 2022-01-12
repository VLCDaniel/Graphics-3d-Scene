#pragma once

// VARIABLES
GLint winWidth = 800;
GLint winHeight = 600;

// Mouse Input
double mouseOffsetX = 0.0f;
double mouseOffsetY = 0.0f;
double lastMouseX = 0.0f;
double lastMouseY = 0.0f;
bool firstMouse = true;

std::vector<GLuint*> shaders;
std::vector<Texture*> textures;
std::vector<Material*> materials;
std::vector<Model*> models;
std::vector<glm::vec3*> lights;
Camera* camera;


// ENUMERATIONS
enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_CONTAINER = 0, TEX_CONTAINER_SPECULAR = 1, TEX_PUSHEEN = 2, TEX_PUSHEEN_SPECULAR = 3 };
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0, MESH_TRIANGLE = 1, MESH_PYRAMID = 2 };


// INITIALISATIONS

// INIT MATRICES - WORLD TRANSFORMATIONS
/* World space -> View space, transform World-space coordinates with ViewMatrix to
				  coordinates that are in front of the user view(camera perspective) */
glm::mat4 ViewMatrix(1.0f);

/* View space -> Clip space, project coordinates within a given range(frustum) back
				 to the Normalized Device Coordinates (-1, 1) with ProjectionMatrix
*/

glm::mat4 ProjectionMatrix(1.0f);
float fov = 90.f; // field of view value -> how large the viewspace is, realistic view = 45 deg
float nearPlane = 0.1f;
float farPlane = 1000.f;

void initMatrices()
{
	// ClipSpace -> fov, aspect ratio (from the viewport), near and far plan of the frustum
	ProjectionMatrix = glm::perspective(glm::radians(fov), (float)winWidth / (float)winHeight, nearPlane, farPlane);
}

void initShaders()
{
	shaders.push_back(new GLuint(LoadShaders("Source.vert", "Source.frag")));
}

void initTextures()
{
	// TEXTURE 0
	textures.push_back(new Texture("Images/container.jpg", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/container_specular.jpg", GL_TEXTURE_2D));

	// TEXTURE 1
	textures.push_back(new Texture("Images/pusheen2.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/pusheen2_specular.png", GL_TEXTURE_2D));

	// Sofa 4 5
	textures.push_back(new Texture("Images/sofa.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/sofa_specular.png", GL_TEXTURE_2D));

	// Nightstand 6 7
	textures.push_back(new Texture("Images/nightstand.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/nightstand_specular.png", GL_TEXTURE_2D));

	// Table 8 9
	textures.push_back(new Texture("Images/table.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/table_specular.png", GL_TEXTURE_2D));

	// Globe 10 11
	textures.push_back(new Texture("Images/globe.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/globe_specular.png", GL_TEXTURE_2D));

	// Chair 12 13
	textures.push_back(new Texture("Images/chair.jpg", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/chair_specular.png", GL_TEXTURE_2D));

	// Floor 14 15
	textures.push_back(new Texture("Images/floor.jpg", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/floor_specular.png", GL_TEXTURE_2D));

	// Lamp 16 17
	textures.push_back(new Texture("Images/lamp.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/lamp_specular.png", GL_TEXTURE_2D));
}

void initMaterials()
{
	materials.push_back(new Material(glm::vec3(0.1f), // ambient light
		glm::vec3(1.f), // diffuse light
		glm::vec3(2.5f, 0.f, 0.f), // specular light
		0, // textures
		1));
}

void initModels()
{
	std::vector<Mesh*> meshes;

	models.push_back(new Model(
		glm::vec3(0.2f, 0.f, -0.2f),
		glm::vec3(1.f),
		materials[0],
		textures[4],
		textures[5],
		"OBJFiles/Sofa.obj"
	));

	models.push_back(new Model(
		glm::vec3(1.2, 0.f, 0.f),
		glm::vec3(1.0f),
		materials[0],
		textures[6],
		textures[7],
		"OBJFiles/Nightstand.obj"
	));

	models.push_back(new Model(
		glm::vec3(0., 0.f, 1.7f),
		glm::vec3(0.25f),
		materials[0],
		textures[8],
		textures[9],
		"OBJFiles/Table.obj"
	));

	models.push_back(new Model(
		glm::vec3(0., 0.55f, 1.7f),
		glm::vec3(0.02f),
		materials[0],
		textures[10],
		textures[11],
		"OBJFiles/Globe.obj"
	));

	models.push_back(new Model(
		glm::vec3(-0.2, 0.f, 2.7f),
		glm::vec3(1.f),
		materials[0],
		textures[12],
		textures[13],
		"OBJFiles/Chair.obj"
	));
	models[models.size() - 1]->rotate(glm::vec3(0.f, 90.f, 0.f));

	models.push_back(new Model(
		glm::vec3(2.2, 0.f, 0.7f),
		glm::vec3(1.f),
		materials[0],
		textures[12],
		textures[13],
		"OBJFiles/Chair.obj"
	));

	models[models.size() - 1]->rotate(glm::vec3(0.f, 270.f, 0.f));

	models.push_back(new Model(
		glm::vec3(0.f, -0.12f, 0.f),
		glm::vec3(0.01f),
		materials[0],
		textures[14],
		textures[15],
		"OBJFiles/Floor.obj"
	));

	models.push_back(new Model(
		glm::vec3(0.7, 0.55f, 1.7f),
		glm::vec3(1.0f),
		materials[0],
		textures[16],
		textures[17],
		"OBJFiles/Lamp.obj"
	));
	
	/*
	Triangle tempTriangle = Triangle();
	meshes.push_back(new Mesh(&tempTriangle,
		glm::vec3(0.0f),
	  glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);

	Pyramid tempPyramid = Pyramid();
	meshes.push_back(new Mesh(&tempPyramid,
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);

	Cube tempCube = Cube();
	meshes.push_back(new Mesh(&tempCube,
		glm::vec3(0.0f),
	  glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);

	models.push_back(new Model(
		glm::vec3(0.f),
		materials[0],
		textures[TEX_CONTAINER],
		textures[TEX_CONTAINER_SPECULAR],
		meshes
	));

	Quad tempQuad = Quad();
	meshes.push_back(new Mesh(&tempQuad,
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);

	models.push_back(new Model(
		glm::vec3(1.5f, 0.f, 0.f),
		materials[0],
		textures[TEX_PUSHEEN],
		textures[TEX_PUSHEEN_SPECULAR],
		meshes
	)); */

	for (auto*& i : meshes)
		delete i;

	meshes.clear();
}

void initLights()
{
	//lights.push_back(new glm::vec3(0.f, 0.f, 3.f));
	lights.push_back(new glm::vec3(1.7f, 0.8f, 1.7f));
}

void initCamera()
{
	camera = new Camera(glm::vec3(1.f, 1.5f, 4.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));
	//camera = new Camera(glm::vec3(1.7f, 0.7f, 1.7f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));
}

void initUniforms()
{
	// INIT UNIFORMS
	// Specify the value of the uniform texture variables for the current program object
	glUseProgram(*shaders[SHADER_CORE_PROGRAM]); // make sure we use our program(shader)

	// Assign World Matrices
	glUniformMatrix4fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	// Assign light and camera position
	glUniform3fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "lightPos0"), 1, glm::value_ptr(*lights[0]));
	

	glUseProgram(0); // Exit shader program
}

void freeMemory()
{
	for (size_t i = 0; i < shaders.size(); i++)
		delete shaders[i];

	for (size_t i = 0; i < textures.size(); i++)
		delete textures[i];

	for (size_t i = 0; i < materials.size(); i++)
		delete materials[i];

	for (auto*& i : models)
		delete i;

	for (size_t i = 0; i < lights.size(); i++)
		delete lights[i];
}

// Update Uniforms
void updateUniforms()
{
	// Update view matrix (camera)
	//ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
	ViewMatrix = camera->getViewMatrix();
	glUniform3fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "lightPos0"), 1, glm::value_ptr(camera->getPosition()));
	glUniformMatrix4fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniform3fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "cameraPos"), 1, glm::value_ptr(camera->getPosition()));
}