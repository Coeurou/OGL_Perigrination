#version 410 core

struct DirectionnalLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 direction;
};

struct Material
{	
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};

in vec3 vsPosition;
in vec2 vsTexCoords;
in vec3 vsNormal;
out vec4 fColor;

uniform DirectionnalLight light;
uniform Material material;

void main()
{
	vec3 unitNormal = normalize(vsNormal);
	vec3 unitPosition = normalize(vsPosition);
	vec3 reflectedLight = reflect(-light.direction, unitNormal);
	
	float diffuseContribution = max(0.0, dot(-light.direction, unitNormal));	
	float specularContribution = pow(max(0.0, dot(reflectedLight, unitPosition)), material.shininess);
	
	vec3 ambientColor = light.ambientColor * material.ambientColor;
	vec3 diffuseColor = light.diffuseColor * (diffuseContribution * material.diffuseColor);
	vec3 specularColor = light.specularColor * (specularContribution * material.specularColor);
	
	vec3 result = ambientColor + diffuseColor + specularColor;
	
	fColor = vec4(result, 1);
}