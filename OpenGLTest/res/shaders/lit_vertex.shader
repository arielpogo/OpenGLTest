#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 v_Normal;
out vec3 v_FragmentPosition;
out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main() {
	gl_Position = u_MVP * vec4(pos, 1.0f);
	v_FragmentPosition = vec3(modelMatrix * vec4(pos, 1.0f)); //transform the vertex position into view coordinates for the fragment shader
	v_Normal = normalMatrix * normal;
	v_TexCoord = texCoord;
};