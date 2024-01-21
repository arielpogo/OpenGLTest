#version 330 core

in vec3 v_Normal;
in vec3 v_FragmentPosition;
in vec2 v_TexCoord;
in vec3 v_LightPosViewSpace;

out vec4 v_FragColor;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
//uniform vec3 u_ViewPos;

uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);

	//ambient lighting - This is like the "default" or "guaranteed" lighting. Everything is at least slightly lit (instead of expensive global illum. calculations)
	float ambientStrength = 0.1f;
	vec3 ambientColor = ambientStrength * u_LightColor;

	//diffuse lighting
	//Forgetting to normalize direction and other relavent vectors for calculations is a common mistake
	vec3 normalized_normal = normalize(v_Normal);
	vec3 lightDirection = normalize(v_LightPosViewSpace - v_FragmentPosition); //get a direction/normalized vector from the light to the fragment

	float diffusionFactor = max(dot(normalized_normal, lightDirection), 0.0f);
	vec3 diffuseColor = diffusionFactor * u_LightColor;

	//specular lighting
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(vec3(0.0f, 0.0f, 0.0f) - v_FragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normalized_normal); //reflect the light off the normal

	//Last integer is the shininess value, the power it's raised to
	float specularComponent = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 256); //dot product -> ensure not negative -> raise to shininess power
	vec3 specularColor = specularStrength * specularComponent * u_LightColor;

	vec3 result = (ambientColor + diffuseColor + specularColor) * u_ObjectColor;
	v_FragColor = vec4(result, 1.0)* texColor;
};