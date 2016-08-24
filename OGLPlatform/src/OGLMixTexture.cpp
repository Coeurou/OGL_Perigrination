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
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glDisable(GL_DEPTH_TEST);
}

bool OGLMixTexture::InitGUI()
{
    return true;
}

bool OGLMixTexture::InitShaders()
{
    bool res = true;
    
    gs::Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.SetSource("multiTexture.vert");
    res &= vertexShader.Compile();
    
    gs::Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.SetSource("multiTexture.frag");
    res &= fragmentShader.Compile();
    
    auto program = std::make_shared<gs::Program>();
    program->Attach(vertexShader.get());
    program->Attach(fragmentShader.get());
    program->Link();
    program->Use();
    programs.push_back(program);

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
    renderedObjs.push_back(std::make_unique<OGLQuad>());
    renderedObjs[0]->InitVertices(glm::vec3());
    
    renderedObjs.push_back(std::make_unique<OGLCube>());
    renderedObjs[1]->InitVertices(glm::vec3());
    
    return (renderedObjs.size() == 2);
}

bool OGLMixTexture::InitVBO()
{
    auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
    vbo->BindVBO();
    auto& quad = renderedObjs[0]->GetVertices();
    auto& cube = renderedObjs[1]->GetVertices();
    glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * (quad.size() + cube.size()), nullptr, GL_STATIC_DRAW);
    
    auto offset = sizeof(gs::Vertex) * quad.size();
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset, quad.data());
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(gs::Vertex) * cube.size(), cube.data());
    vbos.push_back(vbo);
    
    return (vbo != 0);
}

bool OGLMixTexture::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
        res &= InitShaders();
        
        GLint samplerALocation = glGetUniformLocation(programs[0]->get(), "samplerA");
        glUniform1i(samplerALocation, 0);
        GLint samplerBLocation = glGetUniformLocation(programs[0]->get(), "samplerB");
        glUniform1i(samplerBLocation, 1);
        
        mvpLocation = glGetUniformLocation(programs[0]->get(), "MVP");
        mixLocation = glGetUniformLocation(programs[0]->get(), "opacity");
        
        res &= InitTextures();
        res &= InitGeometry();
        res &= InitVBO();
        
        glEnableVertexAttribArray(0); // TODO A placer dans le VAO c'est lui qui doit gerer les vertexattrib du vertexShader
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
        
        glEnable(GL_DEPTH_TEST);
	}
	return res;
}

void OGLMixTexture::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    
	glm::vec4 bgColor(1.0f, 0.87f, 0.41f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	float mix = sinf((float)time * 0.33f) * 0.5f + 0.5f;
    glUniform1f(mixLocation, mix);
    
    // Draw quad
    glm::mat4 MVP = projection *
    glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -5.0f));
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
    
    textures[1]->BindTexture(GL_TEXTURE1);
    textures[0]->BindTexture(GL_TEXTURE0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw cube
    MVP *= glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), mix * 20.0f, glm::vec3(0,1,0));
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
    
    textures[3]->BindTexture(GL_TEXTURE1);
    textures[2]->BindTexture(GL_TEXTURE0);
    
    glDrawArrays(GL_TRIANGLES, 4, 36);
}
