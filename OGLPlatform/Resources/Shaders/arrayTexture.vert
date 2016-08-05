#version 410 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoords;
layout (location=2) in int renderIndex;
layout (location=3) in vec2 offset;

out vec2 vsTexCoords;
flat out int renderInd;

layout (std140) uniform Matrices
{
	mat4 MVP;
    int nbTextureArray;
};

void main()
{
	vec4 finalPos = vec4(position.x + offset.x, position.y + offset.y, position.z, 1);
	gl_Position = MVP * finalPos;
	vsTexCoords = texCoords;
    renderInd = renderIndex % nbTextureArray;
}