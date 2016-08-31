#version 410 core

struct DirectionnalLight
{
	vec3 direction;
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct Material
{
	vec4 diffuseColor;
	vec4 ambientColor;
	vec4 specularColor;
	vec4 emissiveColor;
	float shininess;
};

in vec2 vsTexCoords;
in vec3 vsNormal;
out vec4 fColor;

uniform DirectionnalLight light;
uniform Material material;

uniform sampler2D samplerDiffuse1;
uniform sampler2D samplerDiffuse2;
uniform sampler2D samplerDiffuse3;
uniform sampler2D samplerSpecular1;
uniform sampler2D samplerSpecular2;


void main()
{
	float diffuseContribution = max(0.0, dot(-light.direction, normalize(vsNormal)));
	
	vec4 ambientColor = vec4(light.color, 1) * material.ambientColor;
	vec4 diffuseColor = vec4(light.color, 1) * material.diffuseColor * diffuseContribution;
	fColor = texture(samplerDiffuse1, vsTexCoords) * (ambientColor + diffuseColor);
}