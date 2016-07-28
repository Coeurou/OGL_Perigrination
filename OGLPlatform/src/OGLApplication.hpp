#pragma once

#include <GL/glew.h>
#include "Uncopyable.hpp"
#include "OGLglfw.hpp"
#include "OGLStage.hpp"
#include "FactoryStage.hpp"
#include "OGLApplicationSettings.hpp"
#include <memory>
#include <vector>

class OGLApplication : public Uncopyable
{
public:
	OGLApplication();
	~OGLApplication();

	bool Init();
	int Run();
    
    void SetApplicationTitle(const std::string& title) { glfwApp->SetWindowTitle(title); }
    
private:
	void InitStagesTweakBar();
	void LoadSettings();
	static void TW_CALL ChangeStage(void* newStage);
	void ChangeStage(STAGES newStage);

    std::unique_ptr<OGLglfw> glfwApp;
	std::shared_ptr<OGLStage> stage;
	std::unique_ptr<OGLAntTweakBar> atbApp;
	OGLApplicationSettings settings;
};

class OGLApplicationParams
{
public:
	OGLApplicationParams() {}
	OGLApplicationParams(OGLApplication* application, STAGES stage) : app(application), newStage(stage) {}
	~OGLApplicationParams() {}
	
	OGLApplication* GetApp() { return app; }
	STAGES GetNewStage() { return newStage; }

private:
	OGLApplication* app;
	STAGES newStage;
};