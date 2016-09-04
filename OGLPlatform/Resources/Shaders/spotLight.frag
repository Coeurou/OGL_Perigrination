#version 410 core

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	
	float innerCutOff;
	float outerCutOff;
};

struct Material
{
	float shininess;
};

in vec2 vsTexCoords;
in vec3 vsPosition;
in vec3 vsNormal;
out vec4 fColor;

uniform SpotLight light;
uniform Material material;

uniform mat4 viewMatrix;

uniform sampler2D samplerDiffuse1;
uniform sampler2D samplerDiffuse2;
uniform sampler2D samplerDiffuse3;
uniform sampler2D samplerSpecular1;
uniform sampler2D samplerSpecular2;


void main()
{
	vec3 lightPos = vec3(viewMatrix * vec4(light.position, 1));
	vec3 unitNormal = normalize(vsNormal);
	vec3 unitLight = normalize(lightPos - vsPosition);
	
	vec4 ambientColor = vec4(light.ambientColor, 1);
	
	float theta = dot(unitLight, -light.direction);
	float epsilon = light.innerCutOff - light.outerCutOff;
	float spotIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 reflectedLight = reflect(unitLight, unitNormal);
	float diffuseContribution = max(0.0, dot(unitLight, unitNormal));
	float specAngle = dot(normalize(-vsPosition), reflectedLight);
	float specularContribution = pow(max(0.0, specAngle), material.shininess);
	
	
	vec4 diffuseColor = vec4(light.diffuseColor, 1) * diffuseContribution;
	vec4 specularColor = vec4(light.specularColor, 1) * specularContribution;
	
	vec4 diffuseTexColor = (texture(samplerDiffuse1, vsTexCoords) + texture(samplerDiffuse2, vsTexCoords) +
							texture(samplerDiffuse3, vsTexCoords)) * (ambientColor + diffuseColor);
							
	vec4 specularTexColor = (texture(samplerSpecular1, vsTexCoords) + texture(samplerSpecular2, vsTexCoords)) * specularColor;
	
	fColor = diffuseTexColor * spotIntensity + specularTexColor * spotIntensity;
}