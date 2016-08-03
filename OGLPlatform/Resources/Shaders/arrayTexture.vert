#version 430 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoords;

out vec2 vsTexCoords;

layout (std140) uniform Matrices
{
	mat4 MVP;
};

void main()
{
	gl_Position = MVP * vec4(position, 1);
	vsTexCoords = texCoords;
}