#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices=131) out;

uniform mat4 MVP;

uniform int divisions;
uniform int dist;

out vec3 gColor;

void main()
{
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position;

	vec3 normal = cross((v1.xyz - v0.xyz), (v2.xyz - v0.xyz));
	vec3 N = normalize(normal);

	float dx = abs(v1.x - v0.x) / divisions;
	float dz = abs(v2.z - v0.z) / divisions;

	float x = v0.x;
	float z = v0.z;

	for (int i = 0; i < divisions; i++) {
		for (int j = 0; j < divisions; j++) {
			
			x = v0.x + (j*dx);
			z = v0.z + (i*dz);

			gl_Position = MVP * vec4(x,0,z,1);
			gColor = vec3(1.0, 1.0, 1.0); 
			EmitVertex();
			gl_Position = MVP * vec4(x+dx,0,z,1); 
			gColor = vec3(1.0, 1.0, 1.0); 
			EmitVertex();
			gl_Position = MVP * vec4(x,0,z+dz,1); 
			gColor = vec3(1.0, 1.0, 1.0); 
			EmitVertex();
			gl_Position = MVP * vec4(x+dx,0,z+dz,1); 
			gColor = vec3(1.0, 1.0, 1.0); 
			EmitVertex();

			EndPrimitive();
		}
	}

	for (int i = 0; i < gl_in.length(); i++) {
		vec4 posOffset = gl_in[i].gl_Position + vec4(N, 1);
		gl_Position = MVP * posOffset;
		gColor = vec3(1.0, 1.0, 0.0);
		EmitVertex();
	}
	EndPrimitive();
}