#include "OGLAnimatedTriangle.hpp"
#include "OGLTriangle.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

OGLAnimatedTriangle::OGLAnimatedTriangle()
{}

OGLAnimatedTriangle::~OGLAnimatedTriangle()
{}

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

bool OGLAnimatedTriangle::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='AnimatedTriangle Example' help='This example use glVertexAttrib* to animate a triangle.' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    TwAddVarRW(tweakBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
    TwAddVarRW(tweakBar, "triangleColor", TW_TYPE_COLOR4F, &triangleColor,
               " label='Triangle color' help='Color and transparency of the triangle.' ");
    TwAddButton(tweakBar, "Wireframe mode", SwapPolygonMode, reinterpret_cast<void*>(&polygonMode), " label='Enable wireframe' key=w");
    
    return true;
}

bool OGLAnimatedTriangle::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();

        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("animatedTriangle.vert");
        res &= vertexShader->Compile();
        program->Attach(vertexShader->get());
        
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSource("animatedTriangle.frag");
        res &= fragmentShader->Compile();
        program->Attach(fragmentShader->get());
        
        auto tcsShader = std::make_shared<gs::Shader>(GL_TESS_CONTROL_SHADER);
        tcsShader->SetSource("animatedTriangle.tcs");
        res &= tcsShader->Compile();
        program->Attach(tcsShader->get());
        
        auto tesShader = std::make_shared<gs::Shader>(GL_TESS_EVALUATION_SHADER);
        tesShader->SetSource("animatedTriangle.tes");
        res &= tesShader->Compile();
        program->Attach(tesShader->get());
        
        program->Link();
        program->Use();
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        
        programs.push_back(program);
        vaos.push_back(vao);
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

    glVertexAttrib2fv(0, glm::value_ptr(animation));
	glVertexAttrib4fv(1, glm::value_ptr(triangleColor));

	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	glDrawArrays(GL_PATCHES, 0, 3);
}