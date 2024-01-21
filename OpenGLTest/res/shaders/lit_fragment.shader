#version 330 core

in vec3 v_Normal;
in vec3 v_FragmentPosition;
in vec2 v_TexCoord;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; //Most people calculate this stuff in view space (where the camera is always 0,0,0), TODO: Move everything to the view space instead of model space

uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);

	//ambient lighting - This is like the "default" or "guaranteed" lighting. Everything is at least slightly lit (instead of expensive global illum. calculations)
	float ambientStrength = 0.1f;
	vec3 ambientColor = ambientStrength * lightColor;

	//diffuse lighting
	//Forgetting to normalize direction and other relavent vectors for calculations is a common mistake
	vec3 normalized_normal = normalize(v_Normal);
	vec3 lightDirection = normalize(lightPos - v_FragmentPosition); //get a direction/normalized vector from the light to the fragment

	float diffusionFactor = max(dot(normalized_normal, lightDirection), 0.0f);
	vec3 diffuseColor = diffusionFactor * lightColor;

	//specular lighting
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(viewPos - v_FragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normalized_normal); //reflect the light off the normal

	//Last integer is the shininess value, the power it's raised to
	float specularComponent = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 256); //dot product -> ensure not negative -> raise to shininess power
	vec3 specularColor = specularStrength * specularComponent * lightColor;

	vec3 result = (ambientColor + diffuseColor + specularColor) * objectColor;
	FragColor = vec4(result, 1.0)* texColor;
};