#pragma once

class Mesh
{
private:
	Vertex* vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint type; // type of render -> QUAD/TRIANGLE

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO()
	{
		//CREATE VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// GENERATE BUFFERS
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);



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
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Ox
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Oy
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Oz
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	Mesh(
		Vertex* vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray,
		const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 origin = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		GLuint type = 1
	)
	{
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;
		this->type = type;

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indexArray[i] = indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(
		Primitive* primitive,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 origin = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		GLuint type = 1
	)
	{
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;
		this->type = type;

		this->nrOfVertices = primitive->getNrOfVertices();
		this->nrOfIndices = primitive->getNrOfIndices();

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < this->nrOfVertices; i++)
		{
			this->vertexArray[i] = primitive->getVertices()[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < this->nrOfIndices; i++)
		{
			this->indexArray[i] = primitive->getIndices()[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(const Mesh& obj)
	{
		this->position = obj.position;
		this->origin = obj.origin;
		this->rotation = obj.rotation;
		this->scale = obj.scale;
		this->type = obj.type;

		this->nrOfVertices = obj.nrOfVertices;
		this->nrOfIndices = obj.nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < this->nrOfVertices; i++)
		{
			this->vertexArray[i] = obj.vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < this->nrOfIndices; i++)
		{
			this->indexArray[i] = obj.indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	~Mesh()
	{
		// De-allocate buffers
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);

		delete[] this->vertexArray;
		delete[] this->indexArray;
	}

	// Modifiers
	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	void setOrigin(const glm::vec3 origin)
	{
		this->origin = origin;
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
		glPointSize(10.0);
		//glDrawArrays(GL_POINTS, 0, this->nrOfVertices);

		if (this->nrOfIndices == 0)
		{
			if (this->type == 1)
			{
				glDrawArrays(GL_QUADS, 0, this->nrOfVertices);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
			}

		}
		else
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);

		// Cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};