#pragma once
#include "Uncopyable.hpp"
#include "OGLglfw.hpp"
#include "OGLAntTweakBar.hpp"
#include <memory>

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
    std::unique_ptr<OGLAntTweakBar> atbApp;
};

