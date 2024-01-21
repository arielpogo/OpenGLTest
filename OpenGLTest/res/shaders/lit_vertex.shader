#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 v_Normal;
out vec3 v_FragmentPosition;
out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform mat4 modelMatrix;

void main() {
	gl_Position = u_MVP * vec4(pos, 1.0f);
	v_FragmentPosition = vec3(modelMatrix * vec4(pos, 1.0f)); //transform the vertex position into world coordinates for the fragment shader
	v_Normal = mat3(transpose(inverse(modelMatrix))) * normal; //HEY!!! This is EXPENSIVE!!! Fix this in the future! Do this on the CPU!!
	v_TexCoord = texCoord;
};