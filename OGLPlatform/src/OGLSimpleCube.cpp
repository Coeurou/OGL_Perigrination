#include "OGLSimpleCube.hpp"
#include "OGLCube.hpp"
#include <sstream>

OGLSimpleCube::OGLSimpleCube()
{
}

OGLSimpleCube::~OGLSimpleCube()
{
}

bool OGLSimpleCube::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
		auto tweakBar = GetTweakBar();
		auto barName = TwGetBarName(tweakBar->GetBarByIndex(0));
		std::stringstream format;
		format << barName << " " << " label='SimpleCube Example' ";

		TwDefine(format.str().c_str());
		TwAddVarRW(tweakBar->GetBarByIndex(0), "bgColor", TW_TYPE_COLOR4F, &bgColor,
			" label='Background color' help='Color and transparency of the background.' ");
		TwAddVarRW(tweakBar->GetBarByIndex(0), "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
			" label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
		objRendered = std::make_shared<OGLCube>();
		res &= objRendered->Init();
        
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLSimpleCube::Render(double time)
{
	if (isDynamicBg) {
		float timeElapsed = (float)time;
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
		bgColor.b = 0.5f;
	}
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	objRendered->Render(time);
}
