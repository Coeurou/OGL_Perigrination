#pragma once

#include "OGLObject.hpp"
#include "OGLVertex.hpp"
#include "OGLTexture.hpp"
#include <glm/glm.hpp>

class OGLQuad : public OGLObject
{
public:
	OGLQuad(std::string texImgFilename);
	~OGLQuad();

	bool Init();
	void Render(double time);

	const OGLTexture* GetTexture() const { return &texture; }

private:
	void ComputeGeometry(glm::vec3 origin);	

	std::vector<OGLVertex> geometry;
	OGLTexture texture;
	std::string imgFilename;
};