#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Material.hpp"
#include "Light.hpp"
#include <glm/glm.hpp>

class OGLRefractionReflection : public gs::Stage
{
	gs::Camera camera;
	Skybox skybox;
	float reflectIntensity = 0.5f;
	float refractIntensity = 0.5f;
	float refractIndex1 = 1.0f;
	float refractIndex2 = 1.33f;
	gs::Material material;
	gs::DirectionnalLight sunLight;

public:
	OGLRefractionReflection();
	~OGLRefractionReflection();

	bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;

private:
    bool InitGUI() override;
	void InitMaterial();
	void InitSunLight();
	void SendVSUniforms(const glm::mat4& modelMatrix);
	void SendMaterialUniforms();
	void SendSunLightUniforms();
};