#version 410 core

layout (location=0) in vec3 position;

uniform mat4 MVP;
uniform float time;

const float amplitude = 0.125;
const int frequence = 4;
const float PI = 3.14159;

void main()
{
	float y = amplitude * sin(-PI * frequence * length(position) + time);
	gl_Position = MVP * vec4(position.x, y, position.z, 1);
}