#include "OGLSimpleTriangle.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include <sstream>

OGLSimpleTriangle::OGLSimpleTriangle()
{}

OGLSimpleTriangle::~OGLSimpleTriangle()
{}

bool OGLSimpleTriangle::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='SimpleTriangle Example' help='This example use GL_TRIANGLES.' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    TwAddVarRW(tweakBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
    
    return true;
}

bool OGLSimpleTriangle::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("simpleTriangle.vert");
        res &= vertexShader->Compile();
        program->Attach(vertexShader->get());
        
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSource("simpleTriangle.frag");
        res &= fragmentShader->Compile();
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

void OGLSimpleTriangle::Render(double time)
{
	if (isDynamicBg) {
		float timeElapsed = (float)time;
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
		bgColor.b = 0.5f;
	}
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
