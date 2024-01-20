#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP vec4(pos, 1.0f, 1.0f);
	v_TexCoord = texCoord;
};