#version 410 core

struct DirectionnalLight
{
	float ambientIntensity;
	float diffuseIntensity;
	vec3 color;
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
	
	vec4 ambientColor = vec4(light.color, 1) * light.ambientIntensity;
	vec4 diffuseColor = vec4(light.color, 1) * light.diffuseIntensity * diffuseContribution;
	fColor = texture(sampler, vsTexCoords) * (ambientColor + diffuseColor);
}