#version 410 core

struct Material
{
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
};

struct DirectionnalLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 direction;
};

in vec3 vsEyeDir;
in vec3 vsNormal;

out vec4 fColor;

uniform samplerCube skybox;
uniform float refractionIntensity;
uniform float reflectionIntensity;
uniform float refractIndex1;
uniform float refractIndex2;
uniform Material material;
uniform DirectionnalLight light;

vec4 ComputeReflectionColor()
{
	vec3 unitEye = normalize(vsEyeDir);
	vec3 unitNormal = normalize(vsNormal);
	vec3 reflectedVector = reflect(unitEye, unitNormal);	
	vec3 refractedVector = refract(unitEye, unitNormal, refractIndex1/refractIndex2);
	float fresnelFactor = max(0.0, dot(-unitEye, unitNormal));
	
	return mix(texture(skybox, reflectedVector), texture(skybox, refractedVector), pow(fresnelFactor, refractionIntensity));
}

vec4 ComputeLightContribution()
{
	vec3 unitNormal = normalize(vsNormal);
	float diffuseContribution = max(0.0, dot(-light.direction, unitNormal));
	vec4 ambientColor = material.ambientColor * vec4(light.ambientColor,1);
	vec4 diffuseColor = material.diffuseColor * (vec4(light.diffuseColor,1) * diffuseContribution);
	
	vec3 reflectedLight = reflect(light.direction, unitNormal);
	float specularContribution = pow(max(0.0, dot(-reflectedLight, normalize(vsEyeDir))), material.shininess);
	vec4 specularColor = material.specularColor * (vec4(light.specularColor,1) * specularContribution);
	return ambientColor + diffuseColor + specularColor;
}

void main()
{	
	vec4 reflectedColor = ComputeReflectionColor();
	vec4 enlightedColor = ComputeLightContribution();
	fColor = mix(enlightedColor, reflectedColor, reflectionIntensity);
}