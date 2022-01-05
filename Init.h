#pragma once

// VARIABLES
GLint winWidth = 800;
GLint winHeight = 600;
GLuint ProgramId;

std::vector<GLuint*> shaders;
std::vector<Texture*> textures;
std::vector<Material*> materials;
std::vector<Mesh*> meshes;
std::vector<glm::vec3*> lights;


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
glm::vec3 camPosition(0.f, 0.f, 3.f);
glm::vec3 worldUp(0.f, 1.f, 0.f);
glm::vec3 camFront(0.f, 0.f, -1.f);

/* View space -> Clip space, project coordinates within a given range(frustum) back
				 to the Normalized Device Coordinates (-1, 1) with ProjectionMatrix
*/
glm::mat4 ProjectionMatrix(1.0f);
float fov = 90.f; // field of view value -> how large the viewspace is, realistic view = 45 deg
float nearPlane = 0.1f;
float farPlane = 1000.f;

void initMatrices()
{
	// ViewSpace -> camera position, reference point, world "upwards" direction
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
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
}

void initMaterials()
{
	materials.push_back(new Material(glm::vec3(0.1f), // ambient light
		glm::vec3(1.f), // diffuse light
		glm::vec3(1.f), // specular light
		0, // textures
		1));
}

void initMeshes()
{
	Quad tempQuad = Quad();
	meshes.push_back(new Mesh(&tempQuad,
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);

	Triangle tempTriangle = Triangle();
	meshes.push_back(new Mesh(&tempTriangle,
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);

	Pyramid tempPyramid = Pyramid();
	meshes.push_back(new Mesh(&tempPyramid,
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f))
	);
}

void initLights()
{
	lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void initUniforms()
{
	// INIT UNIFORMS
	// Specify the value of the uniform texture variables for the current program object
	glUseProgram(*shaders[SHADER_CORE_PROGRAM]); // make sure we use our program(shader)

	// Assign World Matrices
	glUniformMatrix4fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	// Assign light and camera position
	glUniform3fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "lightPos0"), 1, glm::value_ptr(*lights[0]));
	glm::vec3 cameraPos(0.f, 0.f, 1.f);
	glUniform3fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "cameraPos"), 1, glm::value_ptr(cameraPos));

	glUseProgram(0); // Exit shader program
}

//void initModels()
//{
//	std::vector<Mesh*>meshes;
//	std::vector<Mesh*>meshes2;
//
//	meshes.push_back(
//		new Mesh(
//			&Pyramid(),
//			glm::vec3(1.f, 0.f, 0.f),
//			glm::vec3(0.f),
//			glm::vec3(0.f),
//			glm::vec3(1.f)
//		)
//	);
//
//	meshes.push_back(
//		new Mesh(
//			&Quad(),
//			glm::vec3(0.f, 0.f, 0.f),
//			glm::vec3(0.f),
//			glm::vec3(0.f),
//			glm::vec3(1.f)
//		)
//	);
//
//	meshes2.push_back(
//		new Mesh(
//			&Quad(),
//			glm::vec3(0.f, 0.f, 0.f),
//			glm::vec3(0.f),
//			glm::vec3(-90.f, 0.f, 0.f),
//			glm::vec3(100.f)
//		)
//	);
//
//	this->models.push_back(new Model(
//		glm::vec3(0.f),
//		this->materials[0],
//		this->textures[TEX_CONTAINER],
//		this->textures[TEX_CONTAINER_SPECULAR],
//		meshes
//	)
//	);
//
//	this->models.push_back(new Model(
//		glm::vec3(0.f, 1.f, 1.f),
//		this->materials[0],
//		this->textures[TEX_PUSHEEN],
//		this->textures[TEX_PUSHEEN_SPECULAR],
//		meshes
//	)
//	);
//
//	this->models.push_back(new Model(
//		glm::vec3(2.f, 0.f, 2.f),
//		this->materials[0],
//		this->textures[TEX_CONTAINER],
//		this->textures[TEX_CONTAINER_SPECULAR],
//		meshes
//	)
//	);
//
//	this->models.push_back(new Model(
//		glm::vec3(2.f, -5.f, 2.f),
//		this->materials[0],
//		this->textures[TEX_CONTAINER],
//		this->textures[TEX_CONTAINER_SPECULAR],
//		meshes2
//	)
//	);
//
//	this->models.push_back(new Model(
//		glm::vec3(4.f, 0.f, 4.f),
//		this->materials[0],
//		this->textures[TEX_CONTAINER],
//		this->textures[TEX_CONTAINER_SPECULAR],
//		"OBJFiles/ddh.obj"
//	)
//	);
//
//	for (auto*& i : meshes)
//		delete i;
//
//	for (auto*& i : meshes2)
//		delete i;
//}

void freeMemory()
{
	for (size_t i = 0; i < shaders.size(); i++)
		delete shaders[i];

	for (size_t i = 0; i < textures.size(); i++)
		delete textures[i];

	for (size_t i = 0; i < materials.size(); i++)
		delete materials[i];

	for (size_t i = 0; i < meshes.size(); i++)
		delete meshes[i];

	for (size_t i = 0; i < lights.size(); i++)
		delete lights[i];
}
