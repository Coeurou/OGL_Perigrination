#include "Application.hpp"
#include "ATBResource.hpp"
#include "EventManager.hpp"
#include "GLFWContext.hpp"
#include "GLFWUtility.hpp"
#include "OGLAntTweakBar.hpp"
#include "OGLApplicationConstants.hpp"
#include "Stage.hpp"
#include "WindowGLFW.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

ATBResource atbHandle;

namespace gs
{
    Application::Application()
    : window(nullptr), stage(new Stage), atbApp(new OGLAntTweakBar)
    {}
    
    Application::~Application()
    {}
    
    bool Application::Init()
    {
        LoadSettings();
		Context* context = new GLFWContext();
        bool res = true;

        window = std::make_unique<WindowGLFW>(context);
		res &= window->CreateWindow();
        
        glewExperimental = GL_TRUE;
        GLenum resGLEW = glewInit();
        res &= (resGLEW == GLEW_OK);
        if (!res)
        {
            std::cerr << "Error: " << glewGetErrorString(resGLEW) << std::endl;
            return res;
        }
        
        res &= atbHandle.InitATB();
        atbApp->InitAntTweakBar("Stages", window->GetWindowWidth(), window->GetWindowHeight());
        InitStagesTweakBar();
        
        //glViewport(0, 0, window->GetWindowWidth(), window->GetWindowHeight());
		projection = glm::perspective(fov, window->GetWindowWidth() / (float)window->GetWindowHeight(), nearDistance, farDistance);

        return res;
    }
    
    int Application::Run()
    {
        while (window->Render()) {
			EventManager::GetInstance()->PollEvents();
			stage->Render(glfwGetTime());
			TwDraw();
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
        assert(stage->Init(window->GetWindowWidth(), window->GetWindowHeight()));
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
        
        OGLApplicationParams* params2 = new OGLApplicationParams(this, STAGES::ANIMATED_PYRAMIDS);
        TwAddButton(mainBar, "AnimatedPyramids", ChangeStage, (void*)params2, " label='Animated pyramids Example' ");
        
        OGLApplicationParams* params3 = new OGLApplicationParams(this, STAGES::MODELMAT);
		TwAddButton(mainBar, "ModelMat", ChangeStage, (void*)params3, " label='Model Matrix Example' ");
        
        OGLApplicationParams* params4 = new OGLApplicationParams(this, STAGES::MIPMAPS);
        TwAddButton(mainBar, "Mipmaps", ChangeStage, (void*)params4, " label='Mipmaps Example' ");        

		OGLApplicationParams* params5 = new OGLApplicationParams(this, STAGES::RIPPLE);
		TwAddButton(mainBar, "ripplePlane", ChangeStage, (void*)params5, " label='Ripple Plane Example' ");
        
        OGLApplicationParams* params6 = new OGLApplicationParams(this, STAGES::GEOMETRYSHADER);
        TwAddButton(mainBar, "GeomShader", ChangeStage, (void*)params6, " label='Geometry Shader Example' ");
        
        OGLApplicationParams* params7 = new OGLApplicationParams(this, STAGES::INDEXEDDRAW);
        TwAddButton(mainBar, "IndexDraw", ChangeStage, (void*)params7, " label='Indexed Draw Example' ");
        
        OGLApplicationParams* params8 = new OGLApplicationParams(this, STAGES::CAMERA);
        TwAddButton(mainBar, "Camera", ChangeStage, (void*)params8, " label='Free Camera Example' ");

		OGLApplicationParams* params9 = new OGLApplicationParams(this, STAGES::SIMPLE_LIGHT);
		TwAddButton(mainBar, "Light", ChangeStage, (void*)params9, " label='Diffuse lighting Example' ");

		OGLApplicationParams* params10 = new OGLApplicationParams(this, STAGES::MIX_TEXTURE);
		TwAddButton(mainBar, "ArrayTex", ChangeStage, (void*)params10, " label='Multi-Texturing Example' ");
		
		OGLApplicationParams* params11 = new OGLApplicationParams(this, STAGES::SKYBOX);
		TwAddButton(mainBar, "Skybox", ChangeStage, (void*)params11, " label='Skybox Example' ");

		OGLApplicationParams* params12 = new OGLApplicationParams(this, STAGES::MATERIAL);
		TwAddButton(mainBar, "Material", ChangeStage, (void*)params12, " label='Material Interactions Example' ");

		OGLApplicationParams* params13 = new OGLApplicationParams(this, STAGES::POINTLIGHT);
		TwAddButton(mainBar, "PointLight", ChangeStage, (void*)params13, " label='Point Light Example' ");

		OGLApplicationParams* params14 = new OGLApplicationParams(this, STAGES::SPOTLIGHT);
		TwAddButton(mainBar, "SpotLight", ChangeStage, (void*)params14, " label='Spot Light Example' ");
        
        OGLApplicationParams* params15 = new OGLApplicationParams(this, STAGES::MULTIPLE_LIGHTS);
        TwAddButton(mainBar, "MultipleLights", ChangeStage, (void*)params15, " label='Multiple Lights Example' ");
        
        OGLApplicationParams* params16 = new OGLApplicationParams(this, STAGES::FRUSTUM_CULLING);
        TwAddButton(mainBar, "FrustumCulling", ChangeStage, (void*)params16, " label='Frustum Culling Example' ");
        
        OGLApplicationParams* params17 = new OGLApplicationParams(this, STAGES::DEPTH_PICKING);
        TwAddButton(mainBar, "DepthPicking", ChangeStage, (void*)params17, " label='Depth Picking Example' ");

		OGLApplicationParams* params18 = new OGLApplicationParams(this, STAGES::COLOR_PICKING);
		TwAddButton(mainBar, "ColorPicking", ChangeStage, (void*)params18, " label='Color Picking Example' ");

		OGLApplicationParams* params19 = new OGLApplicationParams(this, STAGES::REFRACTION_REFLECTION);
		TwAddButton(mainBar, "Refraction_Reflection", ChangeStage, (void*)params19, " label='Refraction/Reflection Example' ");
    }
    
    void Application::LoadSettings()
    {
        settings.LoadSettings(XmlSettingsPath); // TODO load Constants::ApplicationSettingsFilePath
    }
}
