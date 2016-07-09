#include "OGLPointSize.hpp"
#include "OGLPoint.hpp"

OGLPointSize::OGLPointSize()
{
}

OGLPointSize::~OGLPointSize()
{
}

bool OGLPointSize::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
		auto tweakBar = GetTweakBar();

		TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' ");
		TwAddVarRW(tweakBar->GetBarByIndex(0), "cubeColor", TW_TYPE_COLOR4F, &bgColor,
			" label='Background color' help='Color and transparency of the background.' ");
		TwAddVarRW(tweakBar->GetBarByIndex(0), "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
			" label='Dynamic Background' key=b help='Enable dynamic background through time.' ");

		objRendered = std::make_shared<OGLPoint>();
		res &= objRendered->Init();

		glPointSize(40);
	}
	return res;
}

void OGLPointSize::Render(double time)
{
	if (isDynamicBg) {
		float timeElapsed = (float)time;
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
		bgColor.b = 0.5f;
	}
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	glDrawArrays(GL_POINTS, 0, 1);
}
