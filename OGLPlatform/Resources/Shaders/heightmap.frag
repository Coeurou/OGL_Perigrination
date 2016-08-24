#version 410 core

in vec3 vsPosition;
out vec4 fColor;

uniform float heightDivider;


void main()
{	
	fColor = vec4(0, vsPosition.y/heightDivider, 0, 1);
}