#version 410 core

in vec2 vsTexCoords;
out vec4 fColor;

uniform float opacity;
uniform sampler2D samplerA;
uniform sampler2D samplerB;

void main()
{
	vec4 texAContribution = texture(samplerA, vsTexCoords);
	vec4 texBContribution = texture(samplerB, vsTexCoords);
	fColor = mix(texAContribution, texBContribution, opacity);
}