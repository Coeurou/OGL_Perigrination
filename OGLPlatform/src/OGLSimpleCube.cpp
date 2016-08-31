#include "OGLSimpleCube.hpp"
#include "OGLUtility.hpp"
#include "OGLCube.hpp"
#include "Program.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

OGLSimpleCube::OGLSimpleCube()
{}

OGLSimpleCube::~OGLSimpleCube()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glDisable(GL_DEPTH_TEST);
}

bool OGLSimpleCube::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='SimpleCube Example' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    TwAddVarRW(tweakBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
    TwAddVarRW(tweakBar, "Rotation Axis", TW_TYPE_QUAT4F, &rotationAxis[0],
               " label='RotationAxis' ");
    TwAddVarRW(tweakBar, "AutoRotation", TW_TYPE_BOOLCPP, &autoRotate,
               " label='Auto rotation' key=r ");
    
    return true;
}

bool OGLSimpleCube::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();

		resizableObjs.push_back(std::make_unique<OGLCube>());
        res &= resizableObjs[0]->InitVertices(glm::vec3());
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
        gs::Shader vertexShader(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader.SetSource("simpleCube.vert");
        res &= vertexShader.Compile();
        program->Attach(vertexShader.get());
        
        gs::Shader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.SetSource("simpleCube.frag");
        res &= fragmentShader.Compile();
        program->Attach(fragmentShader.get());
        
		res &= program->Link();
        program->Use();
        programs.push_back(program);
        
        auto texture = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
        res &= texture->LoadTexture("Paper_Crumbled.dds");
        glBindTexture(texture->GetTarget(), texture->get());
        textures.push_back(texture);
        
        auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
        vbo->BindVBO();
        auto& data = resizableObjs[0]->GetVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * 36, data.data(), GL_STATIC_DRAW);
        vbos.push_back(vbo);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
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
    
    glm::mat4 rotationMat = projection *
    glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
    rotationMatrix;
    
    GLint rotationLocation = glGetUniformLocation(programs[0]->get(), "MVP");
    glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, glm::value_ptr(rotationMat));
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
