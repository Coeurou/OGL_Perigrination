#pragma once

#include "OGLObject.hpp"
#include <array>
#include <glm/glm.hpp>

enum SIZE { CUBE = 36 };

class OGLCube : public OGLObject
{
public:
	OGLCube();
	~OGLCube();

	bool Init();
	void Render();

private:
	void ComputeGeometry(float width, float height, float depth, glm::vec3 origin);

	std::vector<glm::vec3> geometry;
};

