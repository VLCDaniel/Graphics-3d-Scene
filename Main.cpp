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
Mesh* mesh;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		mesh->move(glm::vec3(0.f, 0.f, -0.01f));
		break;
	case 's':
		mesh->move(glm::vec3(0.f, 0.f, 0.01f));
		break;
	case 'a':
		mesh->move(glm::vec3(-0.01f, 0.f, 0.f));
		break;
	case 'd':
		mesh->move(glm::vec3(0.01f, 0.f, 0.f));
		break;
	case 'q':
		mesh->rotate(glm::vec3(0.f, -1.f, 0.f));
		break;
	case 'e':
		mesh->rotate(glm::vec3(0.f, 1.f, 0.f));
		break;
	case 'z':
		mesh->scaleUp(glm::vec3(1.f));
		break;
	case 'x':
		mesh->scaleUp(glm::vec3(-1.f));
		break;
	case 27: // Esc -> exit
		glutLeaveMainLoop();
		break;
	}
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
	// INIT GL FUNCTIONS
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // specify clear values for the color buffers
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_BLEND); // render images with different levels of transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // factor values for blending
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // How polygons are drawn, try GL_LINE


	// INIT OBJECTS
	CreateShaders();
	Quad tempQuad = Quad();
	//Triangle tempTriangle = Triangle();
	mesh = new Mesh(&tempQuad,
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f));
	texture0 = new Texture("Images/container.jpg", GL_TEXTURE_2D, 0);
	texture1 = new Texture("Images/pusheen2.png", GL_TEXTURE_2D, 1);
	material0 = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0->getTextureUnit(), texture1->getTextureUnit());


	// INIT POSITIONS
	//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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

	// BIND BUFFERS
	glUseProgram(ProgramId); // Use Program (Shader)
	// Bind textures on corresponding texture units
	texture0->bind();
	texture1->bind();
	material0->sendToShader(ProgramId);


	// UPDATE UNIFORMS
	// !!!only if you need to update them each frame, if not, set them in initialization()
	
	//glUniform1i(glGetUniformLocation(ProgramId, "texture0"), texture0->getTextureUnit());
	//glUniform1i(glGetUniformLocation(ProgramId, "texture1"), texture1->getTextureUnit());

	// Update the uniform variable in the shader each frame after calculating the matrix
	//glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));


	// DRAW
	mesh->render(ProgramId);


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