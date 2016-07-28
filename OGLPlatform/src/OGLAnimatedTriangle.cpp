#include "OGLAnimatedTriangle.hpp"
#include "OGLTriangle.hpp"
#include <sstream>

OGLAnimatedTriangle::OGLAnimatedTriangle()
{
}

OGLAnimatedTriangle::~OGLAnimatedTriangle()
{
}

void TW_CALL SwapPolygonMode(void* mode)
{
	auto newMode = static_cast<bool*>(mode);
	if (*newMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	*newMode = !(*newMode);
}

bool OGLAnimatedTriangle::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
		auto tweakBar = GetTweakBar();
		auto barName = TwGetBarName(tweakBar->GetBarByIndex(0));
		std::stringstream format;
		format << barName << " " << " label='AnimatedTriangle Example' help='This example use glVertexAttrib* to animate a triangle.' ";

		TwDefine(format.str().c_str());
		auto stageBar = tweakBar->GetBarByIndex(0);
		TwAddVarRW(stageBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
			" label='Background color' help='Color and transparency of the background.' ");
		TwAddVarRW(stageBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
			" label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
		TwAddVarRW(stageBar, "triangleColor", TW_TYPE_COLOR4F, &triangleColor,
			" label='Triangle color' help='Color and transparency of the triangle.' ");
		TwAddButton(stageBar, "Wireframe mode", SwapPolygonMode, reinterpret_cast<void*>(&polygonMode), " label='Enable wireframe' key=w");

		objRendered = std::make_shared<OGLTriangle>();
		objRendered->ChangeShader(std::pair<SHADER, std::string>(VERTEX, "animatedTriangle.vert"));
		objRendered->ChangeShader(std::pair<SHADER, std::string>(FRAGMENT, "animatedTriangle.frag"));
		objRendered->ChangeShader(std::pair<SHADER, std::string>(TESSCTRL, "animatedTriangle.tcs"));
		objRendered->ChangeShader(std::pair<SHADER, std::string>(TESSEVAL, "animatedTriangle.tes"));
		res &= objRendered->Init();
	}
	return res;
}

void OGLAnimatedTriangle::Render(double time)
{
	float timeElapsed = (float)time;

	if (isDynamicBg) {
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
	}

	animation.x = cosf(timeElapsed) * 0.5f;
	animation.y = sinf(timeElapsed) * 0.5f;

	glVertexAttrib4fv(0, animation.data);
	glVertexAttrib4fv(1, triangleColor.data);

	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	glDrawArrays(GL_PATCHES, 0, 3);
}