#version 410 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

out vec3 vsColor;

uniform mat4 ModelView[5];
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * ModelView[gl_InstanceID] * vec4(position, 1);	
	vsColor = color;
}