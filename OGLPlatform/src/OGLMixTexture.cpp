#include "OGLMixTexture.hpp"
#include "OGLUtility.hpp"
#include "OGLQuad.hpp"
#include "OGLCube.hpp"
#include "Program.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <sstream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


OGLMixTexture::OGLMixTexture()
{}

OGLMixTexture::~OGLMixTexture()
{
	glDisable(GL_PRIMITIVE_RESTART);
	glDisable(GL_DEPTH_TEST);
}

bool OGLMixTexture::InitGUI()
{
    return true;
}

bool OGLMixTexture::InitShaders()
{
    bool res = true;
    
    auto program = std::make_shared<gs::Program>();
	res &= program->CreateShader(GL_VERTEX_SHADER, "multiTexture.vert");
	res &= program->CreateShader(GL_FRAGMENT_SHADER, "multiTexture.frag");
	res &= program->Link();
    programs.push_back(program);

	auto programGround = std::make_shared<gs::Program>();
	res &= programGround->CreateShader(GL_VERTEX_SHADER, "heightmapWave.vert");
	res &= programGround->CreateShader(GL_FRAGMENT_SHADER, "heightmapWave.frag");
	res &= programGround->Link();
	programs.push_back(programGround);

    return res;
}

std::shared_ptr<gs::Texture> CreateTexture(std::string imgFilename)
{
    auto texture = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
    if (!texture->LoadTexture(imgFilename)) {
        return nullptr;
    }
    texture->ChangeParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->ChangeParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    return texture;
}

bool OGLMixTexture::InitTextures()
{
    auto textureQuadA = CreateTexture("Halo.dds");
    if (textureQuadA != nullptr)
        textures.push_back(textureQuadA);
    
    auto textureQuadB = CreateTexture("Light.dds");
    if (textureQuadB != nullptr)
        textures.push_back(textureQuadB);
    
    auto textureCubeA = CreateTexture("Ceiling.dds");
    if (textureCubeA != nullptr)
        textures.push_back(textureCubeA);
    
    auto textureCubeB = CreateTexture("Grass.dds");
    if (textureCubeB != nullptr)
        textures.push_back(textureCubeB);
    
    return (textures.size() == 4);
}

bool OGLMixTexture::InitGeometry()
{
    resizableObjs.push_back(std::make_unique<OGLQuad>());
    resizableObjs[0]->InitVertices(glm::vec3());
    
    resizableObjs.push_back(std::make_unique<OGLCube>());
    resizableObjs[1]->InitVertices(glm::vec3());
    
    return (resizableObjs.size() == 2);
}

bool OGLMixTexture::InitVBO()
{
	auto vao = std::make_shared<gs::VertexArray>();
	vao->BindVAO();
	vaos.push_back(vao);

    auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
    vbo->BindVBO();
    auto& quad = resizableObjs[0]->GetVertices();
    auto& cube = resizableObjs[1]->GetVertices();
	std::vector<glm::vec3> pyramidPositions { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f) };
	std::vector<gs::Vertex> pyramidVertices{ gs::Vertex{ pyramidPositions[0], glm::vec2(0,0) }, gs::Vertex { pyramidPositions[1], glm::vec2(1,0) }, gs::Vertex{ pyramidPositions[4], glm::vec2(0.5f,1.0f) },
											 gs::Vertex{ pyramidPositions[0], glm::vec2(1,0) }, gs::Vertex{ pyramidPositions[2], glm::vec2(0,0) }, gs::Vertex{ pyramidPositions[4], glm::vec2(0.5f,1.0f) },
											 gs::Vertex{ pyramidPositions[2], glm::vec2(1,0) }, gs::Vertex{ pyramidPositions[3], glm::vec2(0,0) }, gs::Vertex{ pyramidPositions[4], glm::vec2(0.5f,1.0f) },
											 gs::Vertex{ pyramidPositions[1], glm::vec2(0,0) }, gs::Vertex{ pyramidPositions[3], glm::vec2(1,0) }, gs::Vertex{ pyramidPositions[4], glm::vec2(0.5f,1.0f) }, 
		gs::Vertex{ pyramidPositions[0], glm::vec2(0,0) }, gs::Vertex{ pyramidPositions[1], glm::vec2(1,0) }, gs::Vertex{ pyramidPositions[2], glm::vec2(0,1) }, 
		gs::Vertex{ pyramidPositions[1], glm::vec2(1,0) }, gs::Vertex{ pyramidPositions[2], glm::vec2(0,1) }, gs::Vertex{ pyramidPositions[3], glm::vec2(1,1) }, };
	

    glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * (quad.size() + cube.size() + pyramidVertices.size()), nullptr, GL_STATIC_DRAW);
    
    auto offsetCube = sizeof(gs::Vertex) * quad.size();
	auto offsetPyramid = offsetCube + sizeof(gs::Vertex) * cube.size();
    glBufferSubData(GL_ARRAY_BUFFER, 0, offsetCube, quad.data());
	glBufferSubData(GL_ARRAY_BUFFER, offsetCube, sizeof(gs::Vertex) * cube.size(), cube.data());
	glBufferSubData(GL_ARRAY_BUFFER, offsetPyramid, sizeof(gs::Vertex) * pyramidVertices.size(), pyramidVertices.data());
    vbos.push_back(vbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	auto vaoGround = std::make_shared<gs::VertexArray>();
	vaoGround->BindVAO();
	vaos.push_back(vaoGround);

	auto vboGround = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
	vboGround->BindVBO();

	std::vector<glm::vec3> groundVertices;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0f, 15.0f);
	for (size_t i = 0; i < nbVertices; i++) {
		for (size_t j = 0; j < nbVertices; j++) {
			float x = ((float)j / (nbVertices-1) * 2 - 1) * halfWorldSize;
			float y = distribution(generator);
			float z = ((float)i / (nbVertices-1) * 2 - 1) * halfWorldSize;
			groundVertices.push_back(glm::vec3(x, y, z));
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * groundVertices.size(), groundVertices.data(), GL_STATIC_DRAW);
	vbos.push_back(vboGround);

	auto iboGround = std::make_shared<gs::VertexBuffer>(GL_ELEMENT_ARRAY_BUFFER);
	iboGround->BindVBO();

	GLushort restartIndex = std::numeric_limits<unsigned short>::max();
	std::vector<GLushort> indices;
	for (GLushort i = 0; i < nbVertices-1; i++) {
		for (GLushort j = 0; j < nbVertices-1; j++) {
			GLushort i0 = j + i * nbVertices;
			GLushort i1 = i0 + nbVertices;
			GLushort i2 = i0 + 1;
			GLushort i3 = i1 + 1;

			if (j == 0) {
				indices.push_back(i0);
				indices.push_back(i1);
			}
			indices.push_back(i2);
			indices.push_back(i3);
			if (j == nbVertices - 2) {
				indices.push_back(restartIndex);
			}
		}
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
	vbos.push_back(iboGround);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(restartIndex);
    return (vbo != 0);
}

bool OGLMixTexture::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
        res &= InitShaders();
        
		programs[0]->Use();
		res &= programs[0]->AddUniform("samplerA");
        glUniform1i(programs[0]->GetUniform("samplerA"), 0);
		res &= programs[0]->AddUniform("samplerB");
        glUniform1i(programs[0]->GetUniform("samplerB"), 1);        
		res &= programs[0]->AddUniform("MVP");
		res &= programs[0]->AddUniform("opacity");

		programs[1]->Use();
		res &= programs[1]->AddUniform("MVP");
		res &= programs[1]->AddUniform("heightDivider");
		res &= programs[1]->AddUniform("time");
		glUniform1f(programs[1]->GetUniform("heightDivider"), 15.0f);

        res &= InitTextures();
        res &= InitGeometry();
        res &= InitVBO();
        
		vaos[0]->BindVAO();
        vbos[0]->BindVBO();
		vaos[0]->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
		vaos[0]->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
		
		vaos[1]->BindVAO();
        vbos[1]->BindVBO();
		vaos[1]->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		camera.SetPosition(glm::vec3(0, 10, -5));
		camera.SetTarget(glm::vec3(0, 10, 0));
		camera.SetSpeed(12.0f);
		camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);
        
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLMixTexture::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    
	camera.Update();

	glm::vec4 bgColor(1.0f, 0.87f, 0.41f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	
	vaos[1]->BindVAO();
    vbos[1]->BindVBO();
    vbos[2]->BindVBO();
	// Draw ground
	programs[1]->Use();
	glm::mat4 MVP = camera.GetViewProjectionMatrix();
	glUniformMatrix4fv(programs[1]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform1f(programs[1]->GetUniform("time"), (float)time);
	vbos[1]->BindVBO();
	glDrawElements(GL_TRIANGLE_STRIP, (int)((nbVertices-1)*2*nbVertices+nbVertices-1), GL_UNSIGNED_SHORT, nullptr);

    // Draw quad    
	vaos[0]->BindVAO();
    vbos[0]->BindVBO();
	programs[0]->Use();
	float mix = sinf((float)time * 0.33f) * 0.5f + 0.5f;
    glUniform1f(programs[0]->GetUniform("opacity"), mix);
	textures[1]->BindTexture(GL_TEXTURE1);
    textures[0]->BindTexture(GL_TEXTURE0);
    
	MVP = glm::translate(MVP, glm::vec3(0, 10, 0));
	glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw cube
	auto model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
	model = glm::rotate(model, mix * 20.0f, glm::vec3(0,1,0));
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP * model));
    
    textures[3]->BindTexture(GL_TEXTURE1);
    textures[2]->BindTexture(GL_TEXTURE0);
    
	glDrawArrays(GL_TRIANGLES, 4, 36);

	for (size_t i = 0; i < 6; i++)
	{
		float sign = (i % 2) * 2 - 1;
		model = glm::translate(glm::mat4(1.0f), glm::vec3(sinf((float)time + i * glm::half_pi<float>()) * 10.0f, 10.0f, i * 20.0f));
		model = glm::rotate(model, glm::radians(45.0f * (float)time), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8));
		glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP * model));
		glDrawArrays(GL_TRIANGLES, 40, 18);
	}
}
