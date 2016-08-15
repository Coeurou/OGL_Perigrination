#version 410 core

uniform mat4 MVP;

void main()
{
	const vec4[4] positions = vec4[4] (vec4(-0.5,-0.5,0.5,1), vec4(0.5,-0.5,0.5,1), vec4(-0.5,0.5,0.5,1), vec4(0.5,0.5,0.5,1));
	gl_Position = MVP * positions[gl_VertexID];
}