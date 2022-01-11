#pragma once

class Model
{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;


public:
	Model(
		glm::vec3 position,
		Material* material,
		Texture* orTexDif,
		Texture* orTexSpec,
		std::vector<Mesh*>& meshes
	)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	// OBJ file loaded model
	Model(
		glm::vec3 position,
		glm::vec3 scale,
		Material* material,
		Texture* orTexDif,
		Texture* orTexSpec,
		const char* objFile
	)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		std::pair<std::vector<Vertex>, std::vector<Vertex>> msh = loadOBJ(objFile);
		std::vector<Vertex> mesh_quad = msh.first;
		std::vector<Vertex> mesh_triangle = msh.second;
		this->meshes.push_back(new Mesh(mesh_quad.data(), mesh_quad.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			scale));

		this->meshes.push_back(new Mesh(mesh_triangle.data(), mesh_triangle.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			scale, 2));

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	//Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}

	void render(GLuint ProgramId)
	{
		// Update the uniforms
		//this->updateUniforms();

		// Update uniforms
		this->material->sendToShader(ProgramId);

		// Use a program (LAST UNIFORM UPDATE UNUSES IT)
		glUseProgram(ProgramId);

		// Draw
		for (auto& i : this->meshes)
		{
			// Activate texture for each mesh
			this->overrideTextureDiffuse->bind(0);
			this->overrideTextureSpecular->bind(1);

			i->render(ProgramId); // Activates shader also
		}
	}
};