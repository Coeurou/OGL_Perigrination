#include "OGLArrayTexture.hpp"
#include "OGLUtils.hpp"
#include "OGLQuad.hpp"
#include <sstream>
#include <random>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct ArrayTextureUniformBuffer
{
	glm::mat4 MVP;
	int nbTextures;
} arrayTextureUB;

OGLArrayTexture::OGLArrayTexture()
{}

OGLArrayTexture::~OGLArrayTexture()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

bool OGLArrayTexture::InitGUI()
{
    return true;
}

bool OGLArrayTexture::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
        auto vao = std::make_shared<OGLVertexArray>();
        vao->Bind();
        vaos.push_back(vao);
        
        OGLShader vertexShader(GL_VERTEX_SHADER);
        vertexShader.SetSource("arrayTexture.vert");
        res &= vertexShader.Compile();
        
        OGLShader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.SetSource("arrayTexture.frag");
        res &= fragmentShader.Compile();
        
        auto program = std::make_shared<OGLProgram>();
        program->Attach(vertexShader.get());
        program->Attach(fragmentShader.get());
        program->Link();
        program->Use();
        programs.push_back(program);
        
        auto texture = std::make_shared<OGLTexture>(IMAGE_TYPE::GLI);
        res &= texture->LoadTexture("Aliens.ktx");
        glBindTexture(texture->getTarget(), texture->get());
        texture->ChangeParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture->ChangeParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        textures.push_back(texture);
        
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
        
        for (size_t i = 0; i < 3; i++) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(-2.0f, 2.0f);
            offsets.push_back(glm::vec2(dist(gen), dist(gen)));
        }

		auto ubo = std::make_shared<OGLVertexBuffer>(GL_UNIFORM_BUFFER);
        ubo->Bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(arrayTextureUB), nullptr, GL_STATIC_DRAW);
		glm::mat4 MVP = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -12.0f));
        
        auto block_i = glGetUniformBlockIndex(program->get(), "Matrices");
        glUniformBlockBinding(program->get(), block_i, 0);
        
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo->get());

		auto buffer = (ArrayTextureUniformBuffer*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(arrayTextureUB), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		buffer->MVP = MVP;
		buffer->nbTextures = texture->getNbTextureInArray();

		glUnmapBuffer(GL_UNIFORM_BUFFER);
        vbos.push_back(ubo);
	}
	return res;
}

void OGLArrayTexture::Render(double time)
{
	glm::vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	for (int i = 0; i < offsets.size(); i++)
	{
        glVertexAttribI1i(2, i);
        glVertexAttrib2fv(3, glm::value_ptr(offsets[i]));
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}
