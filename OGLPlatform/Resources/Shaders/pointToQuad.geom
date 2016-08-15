#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

uniform vec4 modelsMatrices[4];
uniform mat4 VP;

in int instanceID[];

out vec4 tesColor;

void main()
{
	vec4 v0 = gl_in[0].gl_Position;	
	
	gl_Position = VP * (modelsMatrices[instanceID[0]] + vec4(-0.5, -0.5, 0.5, 1.0)); tesColor = vec4(1, 0, 0, 1); EmitVertex();
	gl_Position = VP * (modelsMatrices[instanceID[0]] + vec4(0.5, -0.5, 0.5, 1.0)); tesColor = vec4(0, 1, 0, 1); EmitVertex();
	gl_Position = VP * (modelsMatrices[instanceID[0]] + vec4(-0.5, 0.5, 0.5, 1.0)); tesColor = vec4(0, 0, 1, 1); EmitVertex();
	gl_Position = VP * (modelsMatrices[instanceID[0]] + vec4(0.5, 0.5, 0.5, 1.0)); tesColor = vec4(0.76, 0.8, 0.37, 1); EmitVertex();
	EndPrimitive();
}