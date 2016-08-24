#version 410 core

in vec3 vsColor;
out vec4 fColor;

void main()
{
	fColor = vec4(vsColor, 1);
}