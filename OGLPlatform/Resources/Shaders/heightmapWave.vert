#version 410 core

layout (location=0) in vec3 position;
out vec3 vsPosition;

uniform mat4 MVP;
uniform float time;

const float amplitude = 1.57;
const float frequence = 16;
const float PI = 3.14159;

void main()
{
	float y = amplitude * sin(-PI * frequence * length(position) + time);
	gl_Position = MVP * vec4(position.x, position.y + y, position.z, 1);
	vsPosition = position;
}