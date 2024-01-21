#version 330 core

struct Material {
	vec3 ambientColor; //ambient color, color of the surface under ambient lighting, often the same as the surface's color
	vec3 diffuseColor; //diffuse color,               ""           diffuse
	vec3 specularColor;//specular color,              ""           specular
	float shininess; //scattering/radius of the specular reflection
};

struct Light{
	vec3 position; //in view space
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

in vec3 v_Normal;
in vec3 v_FragmentPosition;
in vec2 v_TexCoord;
//in vec3 v_LightPosViewSpace;

out vec4 v_FragColor;

uniform Material u_Material;
uniform Light u_Light;

uniform sampler2D u_Texture;

void main() {
	//get the texture's color based on the pass texture coordinates and the binded texture (or texture atlas)
	vec4 texColor = texture(u_Texture, v_TexCoord);

	//ambient lighting - This is like the "default" or "guaranteed" lighting. Everything is at least slightly lit (instead of expensive global illum. calculations)
	vec3 ambientColor = u_Light.ambientColor * (vec3(0.1f) * u_Material.ambientColor);

	//diffuse lighting
	//Forgetting to normalize direction and other relavent vectors for calculations is a common mistake
	vec3 normalized_normal = normalize(v_Normal);
	vec3 lightDirection = normalize(u_Light.position - v_FragmentPosition); //get a direction/normalized vector from the light to the fragment

	float diffusionFactor = max(dot(normalized_normal, lightDirection), 0.0f);
	vec3 diffuseColor = u_Light.diffuseColor * diffusionFactor * u_Material.diffuseColor;

	//specular lighting
	vec3 viewDirection = normalize(vec3(0.0f, 0.0f, 0.0f) - v_FragmentPosition); //view pos is 0.0f, ... in view space
	vec3 reflectionDirection = reflect(-lightDirection, normalized_normal); //reflect the light off the normal

	//Last integer is the shininess value, the power it's raised to
	float specularComponent = pow(max(dot(viewDirection, reflectionDirection), 0.0f), u_Material.shininess); //dot product -> ensure not negative -> raise to shininess power factor
	vec3 specularColor = u_Light.ambientColor * specularComponent * u_Material.specularColor;

	vec3 result = ambientColor + diffuseColor + specularColor;
	v_FragColor = vec4(result, 1.0) * texColor;
};