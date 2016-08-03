#version 430 core

in vec vsTexCoords;

uniform sampler2D sampler;

void main()
{
	fColor = texture(sampler, vsTexCoords);
}