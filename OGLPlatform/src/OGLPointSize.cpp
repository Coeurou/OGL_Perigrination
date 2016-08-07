#include "OGLPointSize.hpp"
#include "OGLPoint.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "OGLObjResizable.hpp"
#include <sstream>

OGLPointSize::OGLPointSize()
{}

OGLPointSize::~OGLPointSize()
{}

bool OGLPointSize::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " <<  " label='PointSize Example' help='This example use GL_POINTS & ClearBufferfv.' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    TwAddVarRW(tweakBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
    
    return true;
}

bool OGLPointSize::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
		renderedObjs.push_back(std::make_unique<OGLPoint>(40));
        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("firstPoint.vert");
        res &= vertexShader->Compile();
        fragmentShader->SetSource("firstPoint.frag");
        res &= fragmentShader->Compile();
        program->Attach(vertexShader->get());
        program->Attach(fragmentShader->get());
        program->Link();
        program->Use();
        
        programs.push_back(program);
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
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
    
    renderedObjs[0]->SetWidth(((int)time % 90)+1);
    
	glDrawArrays(GL_POINTS, 0, 1);
}
