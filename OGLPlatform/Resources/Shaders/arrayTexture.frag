#version 430 core

in vec2 vsTexCoords;
in flat int nbTextures;
out vec4 fColor;

uniform sampler2DArray sampler;

void main()
{
	fColor = texture(sampler, vec3(vsTexCoords, float(nbTextures)));
}