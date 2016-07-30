#pragma once

#include "OGLObject.hpp"

class OGLTriangle : public OGLObject
{
public:
	OGLTriangle();
	~OGLTriangle();

	bool Init();
	void Render(double time);
};
