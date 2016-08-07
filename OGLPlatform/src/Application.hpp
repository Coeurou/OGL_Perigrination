#pragma once

#include <GL/glew.h>
#include "Uncopyable.hpp"
#include "GLFWWindow.hpp"
#include "Stage.hpp"
#include "FactoryStage.hpp"
#include "ApplicationSettings.hpp"
#include <memory>
#include <vector>

namespace gs
{
    class Application : public Uncopyable
    {
    public:
        Application();
        ~Application();
        
        bool Init();
        int Run();
        
        void SetApplicationTitle(const std::string& title) { window->SetWindowTitle(title); }
        
    private:
        void InitStagesTweakBar();
        void LoadSettings();
        static void TW_CALL ChangeStage(void* newStage);
        void ChangeStage(STAGES newStage);
        
        std::unique_ptr<GLFWWindow> window;
        std::shared_ptr<Stage> stage;
        std::unique_ptr<OGLAntTweakBar> atbApp;
        ApplicationSettings settings;
    };
    
    class OGLApplicationParams
    {
    public:
        OGLApplicationParams() {}
        OGLApplicationParams(Application* application, STAGES stage) : app(application), newStage(stage) {}
        ~OGLApplicationParams() {}
        
        Application* GetApp() { return app; }
        STAGES GetNewStage() { return newStage; }
        
    private:
        Application* app;
        STAGES newStage;
    };
}