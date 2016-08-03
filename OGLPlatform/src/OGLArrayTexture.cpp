#include "OGLArrayTexture.hpp"
#include "OGLUtils.hpp"
#include "OGLQuad.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

GLuint uniformBuffer = 0;

struct ArrayTextureUniformBuffer
{
	glm::mat4 MVP;
	int nbTextures;
} arrayTextureUB;

OGLArrayTexture::OGLArrayTexture()
{
}

OGLArrayTexture::~OGLArrayTexture()
{
}

bool OGLArrayTexture::Init(int windowWidth, int windowHeight)
{
	bool res = OGLStage::Init(windowWidth, windowHeight);

	if (res) {		
		auto& oglObjs = GetRenderedObjs();
		oglObjs.push_back(std::make_unique<OGLQuad>("Aliens.ktx"));
		oglObjs[0]->ChangeShader(std::pair<SHADER, std::string>(VERTEX, "arrayTexture.vert"));
		oglObjs[0]->ChangeShader(std::pair<SHADER, std::string>(FRAGMENT, "arrayTexture.frag"));
		oglObjs[0]->Init();

		glGenBuffers(1, &uniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(arrayTextureUB), nullptr, GL_DYNAMIC_DRAW);
		glm::mat4 MVP = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

		const auto constQuad = static_cast<const OGLObject*>(oglObjs[0].get());
		auto prog = constQuad->GetProgram()->get();
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBuffer);

		auto buffer = (ArrayTextureUniformBuffer*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(arrayTextureUB), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		buffer->MVP = MVP;
		buffer->nbTextures = dynamic_cast<const OGLQuad*>(constQuad)->GetTexture()->getNbTextureInArray();;

		glUnmapBuffer(GL_UNIFORM_BUFFER);

        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLArrayTexture::Render(double time)
{
	auto& oglObjs = GetRenderedObjs();
	glm::vec4 bgColor(0.2f, 0.18f, 0.18f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, bgColor.data);
		
	for (size_t i = 0; i < oglObjs.size(); i++)
	{
		auto ground = dynamic_cast<const OGLQuad*>(oglObjs[i].get());	
		
		oglObjs[i]->Render(time);
	}
}
