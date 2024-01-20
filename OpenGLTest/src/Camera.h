#pragma once
int globalWidth = 1280;
int globalHeight = 720;

float lastx = 1280.0f / 2;
float lasty = 720.0f / 2;

class Camera {
public:
	void Update() {
		this->viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		this->MVP = projectionMatrix * viewMatrix * modelMatrix;
	}

	glm::vec3 cameraDirection;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)globalWidth / (float)globalHeight, 0.01f, 100.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);//glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
};