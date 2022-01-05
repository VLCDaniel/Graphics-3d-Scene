#pragma once

class Mesh
{
private:
	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO(Vertex* vertexArray,
		const unsigned nrOfVertices,
		GLuint* indexArray,
		const unsigned nrOfIndices)
	{
		// SET VARIABLES
		this->nrOfIndices = nrOfIndices;
		this->nrOfVertices = nrOfVertices;


		/* CREATE VAO
		VAO, VBO, EBO Buffers -> they are send to GPU memory all at once because data from
							     CPU -> GPU is send slow
		Vertex Array Objects(VAO) -> stores VertexAttribPointers so you don't have to bind and 
									 configure buffers every time we draw an object

		Generate VAO Buffer to its id and Bind it to the corresponding VertexArrayBuffer */
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);


		/* Vertex Buffer Objects -> stores vertices, normals, textures, etc.
								    in the high speed memory of video card */
		// Generate VBO Buffer and Bind it to the corresponding ARRAY_BUFFER
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// Any calls on the GL_ARRAY_BUFFER -> configures VBO Buffer
		// Copy Vertex information in the memory Buffer
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);


		// Element Buffer Objects -> stores indices to avoid point overlap
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);


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

	void initVAO(Primitive* primitive)
	{
		// SET VARIABLES
		this->nrOfIndices = primitive->getNrOfIndices();
		this->nrOfVertices = primitive->getNrOfIndices();

		//CREATE VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// GENERATE BUFFERS
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfIndices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfVertices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);


		// SET VertexAttribPointers
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

	void updateUniforms(GLuint ProgramId)
	{
		glUseProgram(ProgramId);
		glUniformMatrix4fv(glGetUniformLocation(ProgramId, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(this->ModelMatrix));
		glUseProgram(0);
	}

	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.0f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Ox
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Oy
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Oz
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	Mesh(
		Vertex* vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray,
		const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)
	)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
		this->updateModelMatrix();
	}

	Mesh(
		Primitive* primitive,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)
	)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(primitive);
		this->updateModelMatrix();
	}

	~Mesh()
	{
		// De-allocate buffers
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

	// Accesors

	// Modifiers
	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	void setRotation(const glm::vec3 rotation)
	{
		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale)
	{
		this->scale = scale;
	}

	// Functions
	void move(const glm::vec3 position)
	{
		this->position += position;
	}

	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
	}

	void scaleUp(const glm::vec3 scale)
	{
		this->scale += scale;
	}

	void update()
	{

	}

	void render(GLint ProgramId)
	{
		// Update Uniforms
		this->updateModelMatrix();
		this->updateUniforms(ProgramId);

		glUseProgram(ProgramId);

		// Bind VAO
		glBindVertexArray(this->VAO); 

		// DRAW
		//glPointSize(10.0);
		//glDrawArrays(GL_POINTS, 0, this->nrOfVertices);

		if(this->nrOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
	}
};