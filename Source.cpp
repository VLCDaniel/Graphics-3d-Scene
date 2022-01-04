#include "Libs.h";


GLint winWidth = 640, winHeight = 480;
GLuint ProgramId, TextureId, SecondTextureId, VAO, VBO, EBO;

Vertex vertices[] =
{
	// Position							// Color					    // Texture
	glm::vec3(0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 1.f),
	glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(1.f, 0.f),
	glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),
	glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.f, 1.f)
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

void LoadTexture(void)
{
	// TEXTURE 1

	// Generate and Bind Texture
	glGenTextures(1, &TextureId);
	glActiveTexture(0); // There are 16 texture units, use the first one (0 by default)
	glBindTexture(GL_TEXTURE_2D, TextureId);

	// Load Image
	int image_Width, image_Height;
	unsigned char* image = SOIL_load_image("Images/container.jpg", &image_Width, &image_Height, 0, SOIL_LOAD_RGBA);


	// Set the texture wrapping/filtering options (on the currently bound texture object)
	// Texture Wrapping -> can't cover entire surface on ox and oy, then repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* Texture Filtering -> low resolution texture on large object (scale texture up or down)
							pixely aspect -> NEAREST; smooth -> LINEAR */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Width, image_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D); // Generates different sizes of texture for different distances
	}
	else
	{
		std::cout << "ERROR::TEXTURE_LOADING_FAILED\n";
		std::cout << SOIL_last_result() << '\n';
		//exit(3);
	}
	// Free memory
	SOIL_free_image_data(image);


	// TEXTURE 2
	glGenTextures(1, &SecondTextureId);
	glBindTexture(GL_TEXTURE_2D, SecondTextureId);

	image = SOIL_load_image("Images/pusheen2.png", &image_Width, &image_Height, 0, SOIL_LOAD_RGBA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Width, image_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE_LOADING_FAILED\n";
		std::cout << SOIL_last_result() << '\n';
		//exit(3);
	}
	SOIL_free_image_data(image);
}

void Initialize(void)
{
	CreateVBO();
	CreateShaders();
	LoadTexture();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // specify clear values for the color buffers

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // How polygons are drawn, try GL_LINE

	// Specify the value of the uniform texture variables for the current program object
	glUseProgram(ProgramId); // make sure we use our program(shader)
	// Assign texture units
	glUniform1i(glGetUniformLocation(ProgramId, "texture0"), 0);
	glUniform1i(glGetUniformLocation(ProgramId, "texture1"), 1);
}

void RenderFunction(void)
{
	// Clear buffers each frame
	// Depth Buffer -> if z value < current z value, don't render pixel
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	// Bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0); // texture unit 0
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glActiveTexture(GL_TEXTURE1); // texture unit 1
	glBindTexture(GL_TEXTURE_2D, SecondTextureId);

	// Bind VAO
	glBindVertexArray(VAO);


	// Use Program (Shader)
	glUseProgram(ProgramId);


	// Draw
	//glPointSize(10.0);
	//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
	glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);


	glutSwapBuffers(); // One buffer is shown, one buffer is drawn
	glFlush(); // Push all buffered operations to OpenGL
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
	//width = winWidth / 10, height = winHeight / 10;
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
	//glutIdleFunc(glutPostRedisplay); // When no events are happening -> redisplay window
	glutKeyboardFunc(processNormalKeys); // Key pressed -> processNormalKeys callback for the current window
	//glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshapeFcn); // When resize window -> callback reshapeFcn
	glutCloseFunc(Cleanup); // When window is destroyed, call Cleanup function
	glutMainLoop(); // Loops main looking for events, keeps window open
}