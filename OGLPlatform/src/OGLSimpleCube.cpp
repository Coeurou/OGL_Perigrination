#include "OGLSimpleCube.hpp"
#include "OGLCube.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

OGLSimpleCube::OGLSimpleCube()
{
}

OGLSimpleCube::~OGLSimpleCube()
{
}

bool OGLSimpleCube::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
		auto tweakBar = GetTweakBar();
        auto stageBar = tweakBar->GetBarByIndex(0);
		auto barName = TwGetBarName(stageBar);
		std::stringstream format;
		format << barName << " " << " label='SimpleCube Example' ";

		TwDefine(format.str().c_str());
		TwAddVarRW(stageBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
			" label='Background color' help='Color and transparency of the background.' ");
		TwAddVarRW(stageBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
			" label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
		TwAddVarRW(stageBar, "Rotation Axis", TW_TYPE_QUAT4F, &rotationAxis[0],
			" label='RotationAxis' ");
		TwAddVarRW(stageBar, "AutoRotation", TW_TYPE_BOOLCPP, &autoRotate,
			" label='Auto rotation' key=r ");

		objRendered = std::make_shared<OGLCube>();
		res &= objRendered->Init();
        
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLSimpleCube::Render(double time)
{
	if (isDynamicBg) {
		float timeElapsed = (float)time;
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
		bgColor.b = 0.5f;
	}
    
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	
	if (autoRotate)
	{
		float angle = glm::radians(45.0f * (float)time);
		rotationAxis = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));		
	}
	glm::mat4 rotationMatrix = glm::toMat4(rotationAxis);

	glm::mat4 rotationMat = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
		rotationMatrix;
	auto cubeObj = dynamic_cast<const OGLCube*>(objRendered.get());

	GLint rotationLocation = glGetUniformLocation(cubeObj->GetProgram()->get(), "rotationMatrix");
	glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, glm::value_ptr(rotationMat));

	objRendered->Render(time);
}
