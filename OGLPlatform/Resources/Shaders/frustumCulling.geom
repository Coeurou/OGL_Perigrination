#version 410 core

layout (points) in;
layout (points, max_vertices=3) out;

uniform mat4 MVP;
uniform vec4 frustumPlanes[6];

bool IsInFrustum(vec3 point)
{
	for (int i = 0; i < 6; i++) {
		vec3 normal = frustumPlanes[i].xyz;
		float dist = frustumPlanes[i].w;
		if (dot(normal, point) + dist < 0.0) {
			return false;
		}
	}
	return true;
}

void main()
{
	for (int i = 0; i < gl_in.length(); i++) {
		vec4 pos = gl_in[i].gl_Position;
		vec3 vertex = vec3(pos.x, pos.y, pos.z);
		gl_Position = MVP * vec4(vertex, 1);

		if (IsInFrustum(vertex)) {
			EmitVertex();
		}
	}
	EndPrimitive();
}