#version 410 core

layout (location=0) in vec3 position;

out vec3 vsTexCoords;

uniform mat4 MVP;

void main()
{
	vec4 mvpPos = MVP * vec4(position, 1);
	gl_Position = mvpPos.xyww;
	vsTexCoords = position;
}