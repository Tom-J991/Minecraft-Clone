#pragma once

#include "Common.h"
#include "Math.h"

struct Camera
{
	float fov;
	float aspectRatio;
	float nearPlane, farPlane;

	float pitch = 0.0f;
	float yaw = -90.0f;
	glm::vec3 position = { 0.0f, 0.0f, 4.0f };

	glm::vec3 front = { 0.0f, 0.0f, -1.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 right;

	glm::mat4 view, proj;

	void Update()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(direction);
		right = glm::normalize(glm::cross(front, up));

		//view = glm::mat4(1.0f);
		view = glm::lookAt(position, position + front, up);

		proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

};
