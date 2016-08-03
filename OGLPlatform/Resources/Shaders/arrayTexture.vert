#version 430 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoords;

out vec2 vsTexCoords;
out flat int nbTextures;

layout (std140,binding=0) uniform Matrices
{
	mat4 MVP;
	int nbTextureArray;
};

void main()
{
	gl_Position = MVP * vec4(position, 1);
	vsTexCoords = texCoords;
	nbTextures = nbTextureArray;
}