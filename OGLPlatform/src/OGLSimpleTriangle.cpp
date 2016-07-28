#include "OGLSimpleTriangle.hpp"
#include "OGLTriangle.hpp"
#include <sstream>

OGLSimpleTriangle::OGLSimpleTriangle()
{
}

OGLSimpleTriangle::~OGLSimpleTriangle()
{
}

bool OGLSimpleTriangle::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
		auto tweakBar = GetTweakBar();
		auto barName = TwGetBarName(tweakBar->GetBarByIndex(0));
		std::stringstream format;
		format << barName << " " << " label='SimpleTriangle Example' help='This example use GL_TRIANGLES.' ";

		TwDefine(format.str().c_str());
		TwAddVarRW(tweakBar->GetBarByIndex(0), "cubeColor", TW_TYPE_COLOR4F, &bgColor,
			" label='Background color' help='Color and transparency of the background.' ");
		TwAddVarRW(tweakBar->GetBarByIndex(0), "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
			" label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
		objRendered = std::make_shared<OGLTriangle>();
		res &= objRendered->Init();
	}
	return res;
}

void OGLSimpleTriangle::Render(double time)
{
	if (isDynamicBg) {
		float timeElapsed = (float)time;
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
		bgColor.b = 0.5f;
	}
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
