#version 410 core

struct DirectionnalLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 direction;
};

in vec2 vsTexCoords;
in vec3 vsNormal;
out vec4 fColor;

uniform sampler2D sampler;
uniform DirectionnalLight light;

void main()
{
	float diffuseContribution = max(0.0, dot(-light.direction, normalize(vsNormal)));
	
	vec3 diffuseColor = light.diffuseColor * diffuseContribution;
	fColor = texture(sampler, vsTexCoords) * vec4(light.ambientColor + diffuseColor, 1);
}