#pragma once

#include <GL/glew.h>
#include "Uncopyable.hpp"
#include "OGLglfw.hpp"
#include "OGLStage.hpp"
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
    std::unique_ptr<OGLglfw> glfwApp;
	std::shared_ptr<OGLStage> stage;
};

