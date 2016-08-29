#version 410 core

in vec3 vsTexCoords;
out vec4 fColor;

uniform samplerCube sampler;

void main()
{
	fColor = texture(sampler, vsTexCoords);
}