#version 330 core

out vec4 v_FragColor;

void main(){
	v_FragColor = vec4(1.0f); //This is to ensure the surface of the emissives don't get caught up in future lighting shader changes
};