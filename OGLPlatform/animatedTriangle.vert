#version 430 core

layout (location=0) in vec4 offset;
layout (location=1) in vec4 color;

out vec4 vsColor;

void main()
{
	const vec4[3] positions = vec4[3] (vec4(-0.5,-0.25,0.5,1), vec4(0.5,-0.25,0.5,1), vec4(0.5,0.5,0.5,1));
	gl_Position = positions[gl_VertexID] + offset;
	
	vsColor = color;
}