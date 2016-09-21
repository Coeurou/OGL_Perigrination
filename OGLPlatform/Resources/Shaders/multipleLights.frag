#version 410 core

struct DirectionnalLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct PointLight
{
	vec3 position;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

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

vec4 ComputeDirLight(DirectionnalLight light, vec3 normal, vec3 viewPos);
vec4 ComputePointLight(PointLight light, vec3 normal, vec3 viewPos);
vec4 ComputeSpotLight(SpotLight light, vec3 normal, vec3 viewPos);

#define NB_POINT_LIGHTS 4

in vec3 vsPosition;
in vec2 vsTexCoords;
in vec3 vsNormal;

out vec4 fColor;

uniform DirectionnalLight dirLight;
uniform PointLight ptLights[NB_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform sampler2D samplerDiffuse1;
uniform sampler2D samplerDiffuse2;
uniform sampler2D samplerDiffuse3;
uniform sampler2D samplerSpecular1;
uniform sampler2D samplerSpecular2;

void main()
{
	vec3 unitNormal = normalize(vsNormal);
	vec4 lightResult;
	lightResult += ComputeDirLight(dirLight, unitNormal, vsPosition);

	for (int i = 0; i < NB_POINT_LIGHTS; i++) {
		lightResult += ComputePointLight(ptLights[0], unitNormal, vsPosition);
	}

	lightResult += ComputeSpotLight(spotLight, unitNormal, vsPosition);

	fColor = lightResult;
}

vec4 ComputeDirLight(DirectionnalLight light, vec3 normal, vec3 viewPos)
{
	vec3 lightDir = normalize(light.direction);
	vec3 reflectedLight = reflect(lightDir, normal);

	float diffuseContribution = max(0.0, dot(lightDir, normal));
	float specularContribution = pow(max(0.0, dot(reflectedLight, normalize(viewPos))), material.shininess);

	vec3 ambientColor = light.ambientColor;
	vec3 diffuseColor = light.diffuseColor * diffuseContribution;
	vec3 specularColor = light.specularColor * specularContribution;

	vec4 diffuseTexColor = (texture(samplerDiffuse1, vsTexCoords) + texture(samplerDiffuse2, vsTexCoords) +
							texture(samplerDiffuse3, vsTexCoords)) * vec4(ambientColor + diffuseColor, 1);
							
	vec4 specularTexColor = (texture(samplerSpecular1, vsTexCoords) + texture(samplerSpecular2, vsTexCoords)) * vec4(specularColor, 1);
	
	return (diffuseTexColor + specularTexColor);
}

vec4 ComputePointLight(PointLight light, vec3 normal, vec3 viewPos)
{
	vec3 lightDir = normalize(light.position - viewPos);
	vec3 reflectedLight = reflect(lightDir, normal);

	float d = length(light.position - viewPos);
	float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
	float diffuseContribution = max(0.0, dot(lightDir, normal));
	float specularContribution = pow(max(0.0, dot(reflectedLight, normalize(viewPos))), material.shininess);

	vec3 ambientColor = light.ambientColor * attenuation;
	vec3 diffuseColor = light.diffuseColor * diffuseContribution * attenuation;
	vec3 specularColor = light.specularColor * specularContribution * attenuation;

	vec4 diffuseTexColor = (texture(samplerDiffuse1, vsTexCoords) + texture(samplerDiffuse2, vsTexCoords) +
							texture(samplerDiffuse3, vsTexCoords)) * vec4(ambientColor + diffuseColor, 1);
							
	vec4 specularTexColor = (texture(samplerSpecular1, vsTexCoords) + texture(samplerSpecular2, vsTexCoords)) * vec4(specularColor, 1);
	
	return (diffuseTexColor + specularTexColor);
}

vec4 ComputeSpotLight(SpotLight light, vec3 normal, vec3 viewPos)
{
	vec3 lightDir = normalize(light.position - viewPos);
	vec3 reflectedLight = reflect(lightDir, normal);

	float theta = dot(lightDir, -light.direction);
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	float diffuseContribution = max(0.0, dot(lightDir, normal));
	float specularContribution = pow(max(0.0, dot(normalize(-viewPos), reflectedLight)), material.shininess);

	vec3 ambientColor = light.ambientColor;
	vec3 diffuseColor = light.diffuseColor * diffuseContribution;
	vec3 specularColor = light.specularColor * specularContribution;

	vec4 diffuseTexColor = (texture(samplerDiffuse1, vsTexCoords) + texture(samplerDiffuse2, vsTexCoords) +
							texture(samplerDiffuse3, vsTexCoords)) * vec4(ambientColor + diffuseColor, 1);
							
	vec4 specularTexColor = (texture(samplerSpecular1, vsTexCoords) + texture(samplerSpecular2, vsTexCoords)) * vec4(specularColor, 1);

	return (diffuseTexColor  * intensity + specularTexColor * intensity);
}