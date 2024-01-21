#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(pos, 1.0f);
	v_TexCoord = texCoord;
};