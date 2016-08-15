#version 410 core

out int instanceID;

void main()
{
	gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
	instanceID = gl_InstanceID;
}