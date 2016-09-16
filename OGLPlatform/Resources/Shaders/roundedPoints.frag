#version 410 core

out vec4 fColor;

void main()
{
	vec2 pos = gl_PointCoord.xy - 0.5;
	if (dot(pos, pos) < 0.25) { discard; }
	fColor = vec4(0, 0, 1, 1);
}