#version 410 core

layout (location=0) in vec3 position;
uniform float time;
uniform float amplitude;

const float PI = 3.141562;

void main()
{
	gl_Position = vec4(position, 1.0) + vec4(0, amplitude * sin(position.x + 2*PI + time), 0, 0);
}