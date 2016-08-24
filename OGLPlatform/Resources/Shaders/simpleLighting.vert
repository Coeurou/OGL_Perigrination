#version 410 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoords;
layout (location=2) in vec3 normal;

out vec2 vsTexCoords;
out vec3 vsNormal;

uniform mat4 MVP;
uniform mat4 NormalMatrix;

void main()
{
	gl_Position = MVP * vec4(position, 1);
	vsTexCoords = texCoords;
	vsNormal = (NormalMatrix * vec4(normal, 0)).xyz;
}