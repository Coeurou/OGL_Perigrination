#version 410 core

in vec2 vsTexCoords;
out vec4 fColor;

uniform float twirlAmount;
uniform float maxRadius;
uniform sampler2D sampler;

void main()
{
	vec2 uv = vsTexCoords - 0.5;
	
	float angle = atan(uv.x, uv.y);	
	float radius = length(uv);
	radius = clamp(radius, 0.0, maxRadius);
	angle += radius * twirlAmount;
	
	vec2 shifted = radius * vec2(cos(angle), sin(angle));
	fColor = texture(sampler, shifted + 0.5);
}