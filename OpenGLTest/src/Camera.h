#pragma once
int globalWidth = 1280;
int globalHeight = 720;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastx = 1280.0f / 2;
float lasty = 720.0f / 2;

float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraDirection;