#include "OGLMipmaps.hpp"
#include "OGLUtility.hpp"
#include "OGLQuad.hpp"
#include "Program.hpp"
#include "Shader.hpp"
#include "Sampler.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Camera.hpp"
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
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
        auto vao = std::make_unique<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(std::move(vao));
        
        gs::Shader vertexShader(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader.SetSource("simpleQuad.vert");
        res &= vertexShader.Compile();
        program->Attach(vertexShader.get());
        
        gs::Shader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.SetSource("simpleQuad.frag");
        res &= fragmentShader.Compile();
        program->Attach(fragmentShader.get());
        
		res &= program->Link();
        program->Use();
        programs.push_back(program);
        
        auto wallTexture = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
        wallTexture->LoadTexture("Wall.dds");
        textures.push_back(std::move(wallTexture));
        
        auto ceilingTexture = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
        ceilingTexture->LoadTexture("Ceiling.dds");
        textures.push_back(std::move(ceilingTexture));
        
        auto wallTextureR = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
        wallTextureR->LoadTexture("Wall.dds");
        textures.push_back(std::move(wallTextureR));
        
        auto floorTexture = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
        floorTexture->LoadTexture("Floor.dds");
        textures.push_back(std::move(floorTexture));
        
        auto sampler = std::make_unique<gs::Sampler>();
        sampler->ChangeParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        sampler->ChangeParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        sampler->BindSampler(0);
        samplers.push_back(std::move(sampler));
        
		resizableObjs.push_back(std::make_unique<OGLQuad>());

        resizableObjs[0]->SetWidth(20);
        resizableObjs[0]->SetHeight(2);
        res &= resizableObjs[0]->InitVertices(glm::vec3());
        
        auto vbo = std::make_unique<gs::VertexBuffer>(GL_ARRAY_BUFFER);
        vbo->BindVBO();
        auto& geomData = resizableObjs[0]->GetVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * geomData.size(), geomData.data(), GL_STATIC_DRAW);
        vbos.push_back(std::move(vbo));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
        
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLMipmaps::Render(double time)
{
	glm::vec4 bgColor(0.2f, 0.18f, 0.18f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	
    samplers[0]->ChangeParameter(GL_TEXTURE_MIN_FILTER, (GLint)texMinFilterVal);
    samplers[0]->ChangeParameter(GL_TEXTURE_MAG_FILTER, (GLint)texMagFilterVal);
    
	for (size_t i = 0; i < textures.size(); i++)
	{
		GLint mvpLocation = glGetUniformLocation(programs[0]->get(), "MVP");

		glm::quat trans(glm::vec3( 0.0f, glm::radians(90.0f), glm::radians(i * 90.0f)));
		glm::mat4 rot = glm::toMat4(trans);

		glm::mat4 MVP = projection *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) * rot;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

		glBindTexture(textures[i]->GetTarget(), textures[i]->get());
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}
