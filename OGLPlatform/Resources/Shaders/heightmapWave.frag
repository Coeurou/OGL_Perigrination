#version 410 core

in vec3 vsPosition;
out vec4 fColor;

uniform float heightDivider;


void main()
{	
	fColor = vec4(vsPosition.y/heightDivider, vsPosition.y/heightDivider, 0.17, 1);
}