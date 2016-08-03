#include "OGLMipmaps.hpp"
#include "OGLUtils.hpp"
#include "OGLQuad.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

OGLMipmaps::OGLMipmaps()
{
}

OGLMipmaps::~OGLMipmaps()
{
}

bool OGLMipmaps::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
		auto tweakBar = GetTweakBar();
        auto stageBar = tweakBar->GetBarByIndex(0);
		auto barName = TwGetBarName(stageBar);
		std::stringstream format;
		format << barName << " " << " label='Textures parameter' ";

		TwDefine(format.str().c_str());
		TwAddVarRW(stageBar, "Min Filter", ATBGLEnumWrapper::GetInstance()->TW_TEXTURE_FILTER, &texMinFilterVal, " label='Minification Filtering' ");
		TwAddVarRW(stageBar, "Mag Filter", ATBGLEnumWrapper::GetInstance()->TW_TEXTURE_FILTER, &texMagFilterVal, " label='Magnification Filtering' ");

		auto& oglObjs = GetRenderedObjs();
		oglObjs.push_back(std::make_unique<OGLQuad>("Wall.dds"));
		oglObjs.push_back(std::make_unique<OGLQuad>("Ceiling.dds"));
		oglObjs.push_back(std::make_unique<OGLQuad>("Wall.dds"));
		oglObjs.push_back(std::make_unique<OGLQuad>("Floor.dds"));

		for (auto& obj : oglObjs) {
			obj->SetWidth(20);
			obj->SetHeight(2);
			res &= obj->Init();
		}
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLMipmaps::Render(double time)
{
	auto& oglObjs = GetRenderedObjs();
	glm::vec4 bgColor(0.2f, 0.18f, 0.18f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, bgColor.data);
		
	for (size_t i = 0; i < oglObjs.size(); i++)
	{
		auto wall = dynamic_cast<const OGLQuad*>(oglObjs[i].get());
		GLint mvpLocation = glGetUniformLocation(wall->GetProgram()->get(), "MVP");

		glm::quat trans(glm::vec3( 0.0f, glm::radians(90.0f), glm::radians(i * 90.0f)));
		glm::mat4 rot = glm::toMat4(trans);

		glm::mat4 MVP = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) * rot;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

		wall->GetTexture()->ChangeParameter(GL_TEXTURE_MIN_FILTER, (GLint)texMinFilterVal);
		wall->GetTexture()->ChangeParameter(GL_TEXTURE_MAG_FILTER, (GLint)texMagFilterVal);
		glBindTexture(wall->GetTexture()->getTarget(), wall->GetTexture()->get());
		oglObjs[i]->Render(time);
	}
}
