#pragma once

#include "OGLObject.hpp"

class OGLPoint : public OGLObject
{
public:
	OGLPoint();
	~OGLPoint();

	bool Init();
	void Render();
};

