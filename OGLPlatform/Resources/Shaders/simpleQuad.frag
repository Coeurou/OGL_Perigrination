#version 410 core

in vec2 vsTexCoords;
out vec4 fColor;

uniform sampler2D sampler;

void main()
{
	fColor = texture(sampler, vsTexCoords);
}