#include "OGLMipmaps.hpp"
#include "OGLQuad.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

OGLMipmaps::OGLMipmaps()
{}

OGLMipmaps::~OGLMipmaps()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glDisable(GL_DEPTH_TEST);
}

bool OGLMipmaps::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Textures parameter' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "Min Filter", ATBGLEnumWrapper::GetInstance()->TW_TEXTURE_FILTER, &texMinFilterVal, " label='Minification Filtering' ");
    TwAddVarRW(tweakBar, "Mag Filter", ATBGLEnumWrapper::GetInstance()->TW_TEXTURE_FILTER, &texMagFilterVal, " label='Magnification Filtering' ");
    
    return true;
}

bool OGLMipmaps::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
        auto vao = std::make_shared<OGLVertexArray>();
        vao->Bind();
        vaos.push_back(vao);
        
        OGLShader vertexShader(GL_VERTEX_SHADER);
        auto program = std::make_shared<OGLProgram>();
        
        vertexShader.SetSource("simpleQuad.vert");
        res &= vertexShader.Compile();
        program->Attach(vertexShader.get());
        
        OGLShader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.SetSource("simpleQuad.frag");
        res &= fragmentShader.Compile();
        program->Attach(fragmentShader.get());
        
        program->Link();
        program->Use();
        programs.push_back(program);
        
        auto wallTexture = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
        wallTexture->LoadTexture("Wall.dds");
        wallTexture->ChangeParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        wallTexture->ChangeParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        textures.push_back(wallTexture);
        
        auto ceilingTexture = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
        ceilingTexture->LoadTexture("Ceiling.dds");
        ceilingTexture->ChangeParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        ceilingTexture->ChangeParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        textures.push_back(ceilingTexture);
        
        auto wallTextureR = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
        wallTextureR->LoadTexture("Wall.dds");
        wallTextureR->ChangeParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        wallTextureR->ChangeParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        textures.push_back(wallTextureR);
        
        auto floorTexture = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
        floorTexture->LoadTexture("Floor.dds");
        floorTexture->ChangeParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        floorTexture->ChangeParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        textures.push_back(floorTexture);
        
		renderedObjs.push_back(std::make_unique<OGLQuad>());

        renderedObjs[0]->SetWidth(20);
        renderedObjs[0]->SetHeight(2);
        res &= renderedObjs[0]->InitVertices(glm::vec3());
        
        auto vbo = std::make_shared<OGLVertexBuffer>(GL_ARRAY_BUFFER);
        vbo->Bind();
        auto geomData = renderedObjs[0]->GetVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(OGLVertex) * geomData.size(), geomData.data(), GL_STATIC_DRAW);
        vbos.push_back(vbo);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, position));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, texCoords));
        
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLMipmaps::Render(double time)
{
	glm::vec4 bgColor(0.2f, 0.18f, 0.18f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
		
	for (size_t i = 0; i < textures.size(); i++)
	{
		GLint mvpLocation = glGetUniformLocation(programs[0]->get(), "MVP");

		glm::quat trans(glm::vec3( 0.0f, glm::radians(90.0f), glm::radians(i * 90.0f)));
		glm::mat4 rot = glm::toMat4(trans);

		glm::mat4 MVP = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) * rot;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

		textures[i]->ChangeParameter(GL_TEXTURE_MIN_FILTER, (GLint)texMinFilterVal);
		textures[i]->ChangeParameter(GL_TEXTURE_MAG_FILTER, (GLint)texMagFilterVal);
		glBindTexture(textures[i]->getTarget(), textures[i]->get());
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}