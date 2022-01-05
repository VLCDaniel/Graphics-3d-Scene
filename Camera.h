#pragma once

enum direction { FORWARD = 0, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera
{
private:
	glm::mat4 ViewMatrix;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	// Euler angles
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	void updateCameraVectors()
	{
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
	{
		this->ViewMatrix = glm::mat4(1.f);

		this->worldUp = worldUp;
		this->position = position;
		this->right = glm::vec3(0.f);
		this->up = worldUp;

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;

		this->updateCameraVectors();
	}

	~Camera() {}

	//Accessors
	const glm::mat4 getViewMatrix()
	{
		this->updateCameraVectors();
		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
		return this->ViewMatrix;
	}

	const glm::vec3 getPosition() const
	{
		return this->position;
	}

	//Functions
	void move(const int direction)
	{
		//Update position vector
		switch (direction)
		{
		case FORWARD:
			this->position += this->front * 0.05f;
			break;
		case BACKWARD:
			this->position -= this->front * 0.05f;
			break;
		case LEFT:
			this->position -= this->right * 0.05f;
			break;
		case RIGHT:
			this->position += this->right * 0.05f;
			break;
		case UP:
			this->position += this->up * 0.05f;
			break;
		case DOWN:
			this->position -= this->up * 0.05f;
			break;
		default:
			break;
		}
	}

	void updatePitch(const float& value)
	{
		this->pitch += value;
		if (this->pitch > 80.f)
			this->pitch = 80.f;
		else if (this->pitch < -80.f)
			this->pitch = -80.f;
	}

	void updateYaw(const float& value)
	{
		this->yaw += value;
		if (this->yaw > 360.f || this->yaw < -360.f)
			this->yaw = 0.f;
	}
};