#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <functional>
#include "OGLApplication.hpp"
#include "OGLApplicationConstants.hpp"

OGLApplication::OGLApplication()
    : glfwApp(new OGLglfw), stage(new OGLStage), atbApp(new OGLAntTweakBar)
{
}

OGLApplication::~OGLApplication()
{
	TwTerminate();
}

bool OGLApplication::Init()
{
	LoadSettings();

	bool res = glfwApp->InitGLFW();
    if (!res) {
        glfwApp = nullptr;
        return res;
    }
    glewExperimental = GL_TRUE;
	GLenum resGLEW = glewInit();
    res &= (resGLEW == GLEW_OK);
	if (!res)
	{
		std::cerr << "Error: " << glewGetErrorString(resGLEW) << std::endl;
        return res;
	}

	res &= (TwInit(TW_OPENGL_CORE, NULL) == 1);
	atbApp->InitAntTweakBar("Stages", glfwApp->GetGLFWWindow().GetWindowWidth(), glfwApp->GetGLFWWindow().GetWindowHeight());
	InitStagesTweakBar();

    return res;
}

int OGLApplication::Run()
{        
    auto loop = [&]() {
		stage->Render(glfwGetTime());
        TwDraw();
    };
    glfwApp->RunMainLoop(loop);
	return 0;
}

void TW_CALL OGLApplication::ChangeStage(void* newParams)
{
	OGLApplicationParams* appParams = static_cast<OGLApplicationParams*>(newParams);
	appParams->GetApp()->ChangeStage(appParams->GetNewStage());
}

void OGLApplication::ChangeStage(STAGES newStage)
{
	stage = FactoryStage::CreateStage(newStage);
	stage->Init(glfwApp->GetGLFWWindow().GetWindowWidth(), glfwApp->GetGLFWWindow().GetWindowHeight());
}

void OGLApplication::InitStagesTweakBar()
{
	std::stringstream format;

	auto mainBar = atbApp->GetBarByIndex(0);

	format << " " << TwGetBarName(mainBar) << " help='This bar allows you to load all the stages representing an OpenGL feature.' position='"
		   << settings.GetStageBarPosition().x << " " << settings.GetStageBarPosition().y << "' ";
	TwDefine(format.str().c_str());

	OGLApplicationParams* params = new OGLApplicationParams(this, STAGES::POINT_SIZE);
	TwAddButton(mainBar, "PointSize", ChangeStage, (void*)params, " label='PointSize Example' ");

	OGLApplicationParams* params1 = new OGLApplicationParams(this, STAGES::SIMPLE_TRIANGLE);
	TwAddButton(mainBar, "SimpleTriangle", ChangeStage, (void*)params1, " label='SimpleTriangle Example' ");

	OGLApplicationParams* params2 = new OGLApplicationParams(this, STAGES::ANIMATED_TRIANGLE);
	TwAddButton(mainBar, "AnimatedTriangle", ChangeStage, (void*)params2, " label='AnimatedTriangle Example' ");
}

void OGLApplication::LoadSettings()
{
	settings.LoadSettings(XmlSettingsPath); // TODO load Constants::ApplicationSettingsFilePath
}
