#version 410 core

layout (location=0) in vec2 offset;
layout (location=1) in vec4 color;

out vec4 vsColor;

void main()
{
	const vec3[3] positions = vec3[3] (vec3(-0.5,-0.25,0.5), vec3(0.5,-0.25,0.5), vec3(0.5,0.5,0.5));
	vec4 vPos = vec4(positions[gl_VertexID].x + offset.x,
					 positions[gl_VertexID].y + offset.y,
					 positions[gl_VertexID].z, 1);
	gl_Position = vPos;
	
	vsColor = color;
}