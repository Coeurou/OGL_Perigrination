#version 410 core

in vec2 vsTexCoords;
flat in int renderInd;

out vec4 fColor;

uniform sampler2DArray sampler;

void main()
{
	fColor = texture(sampler, vec3(vsTexCoords, float(renderInd)));
}