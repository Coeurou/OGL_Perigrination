#pragma once

#include "OGLObject.hpp"
#include "OGLVertex.hpp"
#include "OGLTexture.hpp"
#include <array>
#include <glm/glm.hpp>

enum SIZE { CUBE = 36 };

class OGLCube : public OGLObject
{
public:
	OGLCube();
	~OGLCube();

	bool Init();
	void Render(double time);
    
    const OGLTexture* GetTexture() const { return &texture; }

private:
	void ComputeGeometry(float width, float height, float depth, glm::vec3 origin);

	std::vector<OGLVertex> geometry;
    OGLTexture texture;
};

