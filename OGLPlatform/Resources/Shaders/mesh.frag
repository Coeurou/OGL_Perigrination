#version 410 core

struct DirectionnalLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct Material
{
	float shininess;
};

in vec2 vsTexCoords;
in vec3 vsPosition;
in vec3 vsNormal;
out vec4 fColor;

uniform DirectionnalLight light;
uniform Material material;

uniform sampler2D samplerDiffuse1;
uniform sampler2D samplerDiffuse2;
uniform sampler2D samplerDiffuse3;
uniform sampler2D samplerSpecular1;
uniform sampler2D samplerSpecular2;

vec4 ComputeDirLight(DirectionnalLight light, vec3 normal, vec3 viewPos)
{
	vec3 lightDir = normalize(light.direction);
	vec3 reflectedLight = reflect(lightDir, normal);

	float diffuseContribution = max(0.0, dot(lightDir, normal));
	float specularContribution = pow(max(0.0, dot(reflectedLight, normalize(viewPos))), material.shininess);

	vec3 ambientColor = light.ambientColor;
	vec3 diffuseColor = light.diffuseColor * diffuseContribution;
	vec3 specularColor = light.specularColor * specularContribution;

	vec4 diffuseTexColor = (texture(samplerDiffuse1, vsTexCoords) + texture(samplerDiffuse2, vsTexCoords) +
							texture(samplerDiffuse3, vsTexCoords)) * vec4(ambientColor + diffuseColor, 1);
							
	vec4 specularTexColor = (texture(samplerSpecular1, vsTexCoords) + texture(samplerSpecular2, vsTexCoords)) * vec4(specularColor, 1);
	
	return (diffuseTexColor + specularTexColor);
}

void main()
{
	vec3 unitNormal = normalize(vsNormal);
	
	fColor = ComputeDirLight(light, unitNormal, vsPosition);
}