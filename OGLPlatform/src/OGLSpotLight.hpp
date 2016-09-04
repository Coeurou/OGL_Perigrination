#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"

class OGLSpotLight : public gs::Stage
{
	gs::SpotLight light;
	gs::Camera camera;
	std::vector<glm::vec3> cubePositions;

public:
	OGLSpotLight();
	~OGLSpotLight();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;

private:
	bool InitGUI();
	void InitCubePosition();
	void InitLight();
	bool AddProgram(const std::string& vsFilename, const std::string& fsFilename);
	bool AddMatricesUniform(gs::Program* program);
	bool AddLightUniform(gs::Program* program);
	void SendLightUniform(const gs::Program* program);
	void SendMatricesUniform(const gs::Program* program, glm::mat4 model);
};

