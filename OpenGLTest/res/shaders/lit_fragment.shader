#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
	//ambient lighting - This is like the "default" or "guaranteed" lighting. Everything is at least slightly lit (instead of expensive global illum. calculations)
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 result = ambient * objectColor;
	FragColor = vec4(result, 1.0);
};