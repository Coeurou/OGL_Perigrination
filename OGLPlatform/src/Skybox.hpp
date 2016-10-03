#pragma once
#include "OGLRenderableObject.hpp"
#include "OGLCube.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <memory>



class Skybox : gs::OGLRenderableObject
{
public:
	Skybox();
	~Skybox();

	bool Load(const std::string& cubeMapDirectory) override;
	void Render() override;
	void Render(int nbInstances) override;

	void SetMVP(const glm::mat4& mvp) { mvpMatrix = std::make_unique<glm::mat4>(mvp); }
	void SetCameraPosition(const glm::vec3& camPos) { cameraPos = std::make_unique<glm::vec3>(camPos); }

private:
	gs::Texture texture;
	gs::VertexArray vao;
	gs::VertexBuffer vbo;
	OGLCube cube;
	std::unique_ptr<glm::mat4> mvpMatrix;
	std::unique_ptr<glm::vec3> cameraPos;
};