#pragma once

#include <AntTweakBar.h>

class ATBResource
{
public:
	ATBResource() {}
	~ATBResource() { TwTerminate(); }

	bool InitATB() { return TwInit(TW_OPENGL_CORE, NULL) == 1; }
};
