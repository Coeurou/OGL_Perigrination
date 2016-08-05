#version 410 core

void main()
{
	const vec4[3] positions = vec4[3] (vec4(-0.25,-0.25,0.5,1), vec4(0.25,-0.25,0.5,1), vec4(0.25,0.25,0.5,1));
	gl_Position = positions[gl_VertexID];
}