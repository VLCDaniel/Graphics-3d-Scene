#pragma once

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

public:
	Material(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		GLint diffuseTex,
		GLint specularTex
	)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material() {}

	void sendToShader(GLint ProgramId)
	{
		glUseProgram(ProgramId);

		glUniform3fv(glGetUniformLocation(ProgramId, "material.ambient"), 1, glm::value_ptr(this->ambient));
		glUniform3fv(glGetUniformLocation(ProgramId, "material.diffuse"), 1, glm::value_ptr(this->diffuse));
		glUniform3fv(glGetUniformLocation(ProgramId, "material.specular"), 1, glm::value_ptr(this->specular));

		glUniform1i(glGetUniformLocation(ProgramId, "material.diffuseTex"), this->diffuseTex);
		glUniform1i(glGetUniformLocation(ProgramId, "material.specularTex"), this->specularTex);

		glUseProgram(0);
	}

};