#include "OGLMixTexture.hpp"
#include "OGLUtils.hpp"
#include "OGLQuad.hpp"
#include <sstream>
#include <random>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


OGLMixTexture::OGLMixTexture()
{}

OGLMixTexture::~OGLMixTexture()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

bool OGLMixTexture::InitGUI()
{
    return true;
}

bool OGLMixTexture::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
        auto vao = std::make_shared<OGLVertexArray>();
        vao->Bind();
        vaos.push_back(vao);
        
        OGLShader vertexShader(GL_VERTEX_SHADER);
        vertexShader.SetSource("multiTexture.vert");
        res &= vertexShader.Compile();
        
        OGLShader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.SetSource("multiTexture.frag");
        res &= fragmentShader.Compile();
        
        auto program = std::make_shared<OGLProgram>();
        program->Attach(vertexShader.get());
        program->Attach(fragmentShader.get());
        program->Link();
        program->Use();
        programs.push_back(program);
        
        auto textureA = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
        res &= textureA->LoadTexture("light.dds");
		textureA->Bind(GL_TEXTURE0);
		textureA->ChangeParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		textureA->ChangeParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        textures.push_back(textureA);
		GLint samplerALocation = glGetUniformLocation(program->get(), "samplerA");
		glUniform1i(samplerALocation, 0);

		auto textureB = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
		res &= textureB->LoadTexture("halo.dds");
		textureB->Bind(GL_TEXTURE1);
		textureB->ChangeParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		textureB->ChangeParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		textures.push_back(textureB);
		GLint samplerBLocation = glGetUniformLocation(program->get(), "samplerB");
		glUniform1i(samplerBLocation, 1);

        renderedObjs.push_back(std::make_unique<OGLQuad>());
        renderedObjs[0]->InitVertices(glm::vec3());
        
        auto vbo = std::make_shared<OGLVertexBuffer>(GL_ARRAY_BUFFER);
        vbo->Bind();
        auto quad = renderedObjs[0]->GetVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(OGLVertex) * quad.size(), quad.data(), GL_STATIC_DRAW);
        vbos.push_back(vbo);
        
        glEnableVertexAttribArray(0); // TODO A placer dans le VAO c'est lui qui doit gerer les vertexattrib du vertexShader
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, position));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, texCoords));
        
		auto mvpLoc = glGetUniformLocation(program->get(), "MVP");
		mixLocation = glGetUniformLocation(program->get(), "opacity");
		glm::mat4 MVP = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
						glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));
	}
	return res;
}

void OGLMixTexture::Render(double time)
{
	glm::vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	float mix = sinf((float)time * 0.2f) * 0.5f + 0.5f;
    glUniform1f(mixLocation, mix);
        
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
