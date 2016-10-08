#version 410 core

layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;

out vec3 vsEyeDir;
out vec3 vsNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;
uniform vec3 eyePosition;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);
	vsNormal = vec3(normalMatrix * vec4(normal, 0));
	vsEyeDir = vec3(model * vec4(position, 1)) - eyePosition;
}