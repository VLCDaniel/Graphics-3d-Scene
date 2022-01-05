// Libraries
#include "Libs.h";
// Initialisations
#include "Init.h"

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		camera->move(FORWARD);
		break;
	case 's':
		camera->move(BACKWARD);
		break;
	case 'a':
		camera->move(LEFT);
		break;
	case 'd':
		camera->move(RIGHT);
		break;
	case '[':
		camera->move(UP);
		break;
	case ']':
		camera->move(DOWN);
		break;
	case 'z':
		meshes[MESH_QUAD]->scaleUp(glm::vec3(1.f));
		break;
	case 'x':
		meshes[MESH_QUAD]->scaleUp(glm::vec3(-1.f));
		break;
	case 'f':
		meshes[MESH_QUAD]->move(glm::vec3(0., 0.01, 0.f));
		break;
	case 'g':
		meshes[MESH_QUAD]->move(glm::vec3(0., -0.01, 0.f));
		break;
	case 27: // Esc -> exit
		glutLeaveMainLoop();
		break;
	}
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camera->updatePitch(1.0);
		break;
	case GLUT_KEY_DOWN:
		camera->updatePitch(-1.0);
		break;
	case GLUT_KEY_LEFT:
		camera->updateYaw(-1.0);
		break;
	case GLUT_KEY_RIGHT:
		camera->updateYaw(1.0);
		break;
	}
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
	//glutSetCursor(GLUT_CURSOR_NONE); // Don't display the mouse cursor


	// INIT VARIABLES
	initMatrices();
	initShaders();
	initTextures();
	initMaterials();
	initMeshes();
	initLights();
	initCamera();
	initUniforms();
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers each frame
	glEnable(GL_DEPTH_TEST); // Depth Buffer -> if z value < current z value, don't render pixel


	// BIND BUFFERS
	glUseProgram(*shaders[SHADER_CORE_PROGRAM]); // Use Program (Shader)
	// Bind textures on corresponding texture units
	textures[TEX_CONTAINER]->bind(0);
	textures[TEX_CONTAINER_SPECULAR]->bind(1);

	// UPDATE UNIFORMS
	updateUniforms();
	// !!!only if you need to update them each frame, if not, set them in initialization()
	//glUniform1i(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "texture0"), textures[TEX_PUSHEEN]->getTextureUnit());
	//glUniform1i(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "texture1"), textures[TEX_CONTAINER]->getTextureUnit());

	// Update the uniform variable in the shader each frame after calculating the matrix
	//glUniformMatrix4fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	materials[MAT_1]->sendToShader(*shaders[SHADER_CORE_PROGRAM]);


	// DRAW
	meshes[2]->render(*shaders[SHADER_CORE_PROGRAM]);

	//textures[TEX_PUSHEEN]->bind(1);
	//textures[TEX_CONTAINER]->bind(0);
	//meshes[0]->render(*shaders[SHADER_CORE_PROGRAM]);

	// END DRAW
	glutSwapBuffers(); // One buffer is shown, one buffer is drawn
	glFlush(); // Push all buffered operations to OpenGL

	// RESET
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	glUseProgram(*shaders[SHADER_CORE_PROGRAM]);
	glUniformMatrix4fv(glGetUniformLocation(*shaders[SHADER_CORE_PROGRAM], "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUseProgram(0);
}

void Cleanup(void)
{
	// Destroy Objects / Free Memory
	glDeleteProgram(*shaders[SHADER_CORE_PROGRAM]);
	freeMemory();
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
	glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshapeFcn); // When resize window -> callback reshapeFcn
	glutCloseFunc(Cleanup); // When window is destroyed, call Cleanup function
	glutMainLoop(); // Loops main looking for events, keeps window open
}