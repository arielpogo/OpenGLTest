#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 v_Normal;
out vec3 v_FragmentPosition;
out vec2 v_TexCoord;

uniform mat4 u_MVP; //projectionMatrix * viewMatrix * modelMatrix
uniform mat4 u_MV; //viewMatrix * modelMatrix
//uniform mat4 u_ViewMatrix;
//uniform mat4 projectionMatrix;
uniform mat3 u_NormalMatrix; //viewMatrix * modelMatrix, inversed, transposed

void main() {
	gl_Position = u_MVP * vec4(pos, 1.0f); //avoid matrix multiplication in each shader, by doing in on the CPU. However maybe uniforms are too slow vs just calculating?
	v_FragmentPosition = vec3(u_MV * vec4(pos, 1.0f));				 //get the vertex position into view space for the fragment shader
	v_Normal = u_NormalMatrix * normal;								 //get the normal		   into ""
	v_TexCoord = texCoord;
};