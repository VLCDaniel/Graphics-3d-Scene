#include "Libs.h";


GLint winWidth = 800;
GLint winHeight = 600;
GLuint ProgramId, TextureId, SecondTextureId, VAO, VBO, EBO;

// WORLD TRANSFORMATIONS

/* Local space -> World space, transform objects with ModelMatrix
				  and place them into their world (outside of -1,1 restriction) */
glm::mat4 ModelMatrix(1.0f);
glm::vec3 position(0.f), rotation(0.f), scale(1.f);

/* World space -> View space, transform World-space coordinates with ViewMatrix to
				  coordinates that are in front of the user view(camera perspective) */
glm::mat4 ViewMatrix(1.0f);

/* View space -> Clip space, project coordinates within a given range(frustum) back
				 to the Normalized Device Coordinates (-1, 1) with ProjectionMatrix
*/
glm::mat4 ProjectionMatrix(1.0f);


glm::vec3 camPostion(0.f, 0.f, 1.f), worldUp(0.f, 1.f, 0.f), camFront(0.f, 0.f, -1.f);
float fov = 90.f, nearPlane = 0.1f;


// OBJECTS
Texture* texture0, *texture1;
Material* material0;
Mesh* test;

Vertex vertices[] =
{
	// Position								// Color						// Texture					// Normals
	glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, -1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 3,	// Triangle 1
	1, 2, 3		// Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		position.z -= 0.01f;
		break;
	case 's':
		position.z += 0.01f;
		break;
	case 'a':
		position.x -= 0.01f;
		break;
	case 'd':
		position.x += 0.01f;
		break;
	case 'q':
		rotation.y -= 1.f;
		break;
	case 'e':
		rotation.y += 1.f;
		break;
	case 'z':
		scale += 0.1f;
		break;
	case 'x':
		scale -= 0.1f;
		break;
	case 27: // Esc -> exit
		glutLeaveMainLoop();
		break;
	}
}

void CreateVBO(void)
{
	/* VAO, VBO, EBO Buffers -> they are send to GPU memory all at once because data from
								CPU -> GPU is send slow */


	/* Vertex Array Objects -> stores VertexAttribPointers so you don't have to
								bind and configure buffers every time we draw an object */

	// Generate VAO Buffer to its id and Bind it to the corresponding VertexArrayBuffer
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	/* Vertex Buffer Objects -> stores vertices, normals, textures, etc.
							   in the high speed memory of video card */

							   // Generate VBO Buffer and Bind it to the corresponding ARRAY_BUFFER
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Any calls on the GL_ARRAY_BUFFER -> configures VBO Buffer
	// Copy Vertex information in the memory Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Element Buffer Objects -> stores indices to avoid point overlap
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Set VertexAttribPointers -> specify how to interpret the ArrayBuffer in the shaders
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
	glEnableVertexAttribArray(2);

	// Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void DestroyVBO(void)
{
	// De-allocate buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("Source.vert", "Source.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	// INIT OBJECTS

	CreateVBO();
	CreateShaders();

	// MODEL MESH
	test = new Mesh(vertices, nrOfVertices, indices, nrOfIndices);

	texture0 = new Texture("Images/container.jpg", GL_TEXTURE_2D, 0);
	texture1 = new Texture("Images/pusheen2.png", GL_TEXTURE_2D, 1);
	material0 = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0->getTextureUnit(), texture1->getTextureUnit());

	// INIT GL FUNCTIONS

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // specify clear values for the color buffers
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_BLEND); // render images with different levels of transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // factor values for blending
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // How polygons are drawn, try GL_LINE


	// INIT POSITIONS

	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
	rotation.x = 0.01f;


	// LIGHTS
	glm::vec3 lightPos0(0.f, 0.f, 2.f);
	glm::vec3 cameraPos(0.f, 0.f, 3.f);


	// INIT UNIFORMS

	// Specify the value of the uniform texture variables for the current program object
	glUseProgram(ProgramId); // make sure we use our program(shader)
	// Assign texture units
	glUniform1i(glGetUniformLocation(ProgramId, "texture0"), texture0->getTextureUnit());
	glUniform1i(glGetUniformLocation(ProgramId, "texture1"), texture1->getTextureUnit());

	// Assign World Matrices
	glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	// Assign light and camera postiion
	glUniform3fv(glGetUniformLocation(ProgramId, "lightPos0"), 1, glm::value_ptr(lightPos0));
	glUniform3fv(glGetUniformLocation(ProgramId, "cameraPos"), 1, glm::value_ptr(cameraPos));

	glUseProgram(0); // Exit shader program
}

void RenderFunction(void)
{
	// Clear buffers each frame
	// Depth Buffer -> if z value < current z value, don't render pixel
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// BIND

	// Bind textures on corresponding texture units
	texture0->bind();
	texture1->bind();
	material0->sendToShader(ProgramId);
	glBindVertexArray(VAO); // Bind VAO

	glUseProgram(ProgramId); // Use Program (Shader)



	// UPDATE UNIFORMS
	// Translate, rotate, scale
	//rotation.x += 0.01f;
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Ox
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Oy
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Oz
	ModelMatrix = glm::scale(ModelMatrix, scale);
	// Update the uniform variable in the shader each frame after calculating the matrix
	glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));


	// DRAW

	//glPointSize(10.0);
	//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
	glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

	test->render(ProgramId);


	glutSwapBuffers(); // One buffer is shown, one buffer is drawn
	glFlush(); // Push all buffered operations to OpenGL
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	// Resize Viewport, Width, Height
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;

	// To maintain aspect ratio, recalculate ProjectionMatrix and send it to the shader
	float width = winWidth / 10, height = winHeight / 10;
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	// INIT GLUT
	glutInit(&argc, argv);

	// CREATE WINDOW
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); // RGBA, Depth and double buffer
	glutInitWindowPosition(0, 0); // relative to the screen
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Proiect II");

	// INIT GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED\n";
		glutLeaveMainLoop();
	}

	Initialize();
	glutDisplayFunc(RenderFunction); // Redraw windows when change size, move window, key press, etc.
	glutIdleFunc(glutPostRedisplay); // When no events are happening -> redisplay window
	glutKeyboardFunc(processNormalKeys); // Key pressed -> processNormalKeys callback for the current window
	//glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshapeFcn); // When resize window -> callback reshapeFcn
	glutCloseFunc(Cleanup); // When window is destroyed, call Cleanup function
	glutMainLoop(); // Loops main looking for events, keeps window open
}