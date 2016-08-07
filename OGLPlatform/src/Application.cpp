#include <iostream>
#include <sstream>
#include <functional>
#include "Application.hpp"
#include "GLFWContext.hpp"
#include "GLFWWindow.hpp"
#include "OGLApplicationConstants.hpp"

namespace gs
{
    Application::Application()
    : window(nullptr), stage(new Stage), atbApp(new OGLAntTweakBar)
    {}
    
    Application::~Application()
    {
        TwTerminate();
    }
    
    bool Application::Init()
    {
        LoadSettings();
        gs::Context* context = new GLFWContext();
        bool res = context->InitContext();
        if (!res) {
            return res;
        }
        //window = std::make_unique<GLFWWindow>(*context);
        glewExperimental = GL_TRUE;
        GLenum resGLEW = glewInit();
        res &= (resGLEW == GLEW_OK);
        if (!res)
        {
            std::cerr << "Error: " << glewGetErrorString(resGLEW) << std::endl;
            return res;
        }
        
        res &= (TwInit(TW_OPENGL_CORE, NULL) == 1);
        atbApp->InitAntTweakBar("Stages", window->GetWindowWidth(), window->GetWindowHeight());
        InitStagesTweakBar();
        
        return res;
    }
    
    int Application::Run()
    {
        while (!glfwWindowShouldClose(window->get())) {
            stage->Render(glfwGetTime());
            TwDraw();
            
            glfwSwapBuffers(window->get());
            glfwPollEvents();
        }
        return 0;
    }
    
    void TW_CALL Application::ChangeStage(void* newParams)
    {
        OGLApplicationParams* appParams = static_cast<OGLApplicationParams*>(newParams);
        appParams->GetApp()->ChangeStage(appParams->GetNewStage());
    }
    
    void Application::ChangeStage(STAGES newStage)
    {
        stage = FactoryStage::CreateStage(newStage);
        stage->Init(window->GetWindowWidth(), window->GetWindowHeight());
    }
    
    void Application::InitStagesTweakBar()
    {
        std::stringstream format;
        
        auto mainBar = atbApp->GetBarByIndex(0);
        
        format << " " << TwGetBarName(mainBar) << " help='This bar allows you to load all the stages representing an OpenGL feature.' position='"
        << settings.GetStageBarPosition().x << " " << settings.GetStageBarPosition().y << "' ";
        TwDefine(format.str().c_str());
        
        OGLApplicationParams* params = new OGLApplicationParams(this, STAGES::POINT_SIZE);
        TwAddButton(mainBar, "PointSize", ChangeStage, (void*)params, " label='PointSize Example' ");
        
        OGLApplicationParams* params1 = new OGLApplicationParams(this, STAGES::SIMPLE_TRIANGLE);
        TwAddButton(mainBar, "SimpleTriangle", ChangeStage, (void*)params1, " label='Simple triangle Example' ");
        
        OGLApplicationParams* params2 = new OGLApplicationParams(this, STAGES::ANIMATED_TRIANGLE);
        TwAddButton(mainBar, "AnimatedTriangle", ChangeStage, (void*)params2, " label='Animated triangle Example' ");
        
        OGLApplicationParams* params3 = new OGLApplicationParams(this, STAGES::SIMPLE_CUBE);
        TwAddButton(mainBar, "SimpleCube", ChangeStage, (void*)params3, " label='Simple cube Example' ");
        
        OGLApplicationParams* params4 = new OGLApplicationParams(this, STAGES::MIPMAPS);
        TwAddButton(mainBar, "Mipmaps", ChangeStage, (void*)params4, " label='Mipmaps Example' ");
        
        OGLApplicationParams* params5 = new OGLApplicationParams(this, STAGES::MIX_TEXTURE);
        TwAddButton(mainBar, "ArrayTex", ChangeStage, (void*)params5, " label='Array texture Example' ");
    }
    
    void Application::LoadSettings()
    {
        settings.LoadSettings(XmlSettingsPath); // TODO load Constants::ApplicationSettingsFilePath
    }
}