#include "OGLModelMatrix.hpp"
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

OGLModelMatrix::OGLModelMatrix()
{}

OGLModelMatrix::~OGLModelMatrix()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glDisable(GL_DEPTH_TEST);
}

bool OGLModelMatrix::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='SimpleCube Example' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "rotX", TW_TYPE_INT32, &rX,
               " label='Rotation X' min='-180' max='360'");
    TwAddVarRW(tweakBar, "rotY", TW_TYPE_INT32, &rY,
               " label='Rotation Y' min='-180' max='360'");
    TwAddVarRW(tweakBar, "rotZ", TW_TYPE_INT32, &rZ,
               " label='Rotation Z' min='-180' max='360'");
    
    TwAddVarRW(tweakBar, "T axis", TW_TYPE_DIR3F, &tVector[0],
               " label='Translation Axis' ");
    
    return true;
}

bool OGLModelMatrix::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();

		renderedObjs.push_back(std::make_unique<OGLCube>());
        res &= renderedObjs[0]->InitVertices(glm::vec3());
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
        gs::Shader vertexShader(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader.SetSource("modelMatrix.vert");
        res &= vertexShader.Compile();
        program->Attach(vertexShader.get());
        
        gs::Shader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.SetSource("simpleCube.frag");
        res &= fragmentShader.Compile();
        program->Attach(fragmentShader.get());
        
        program->Link();
        program->Use();
        programs.push_back(program);
        
        auto texture = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
        res &= texture->LoadTexture("Paper_Crumbled.dds");
        glBindTexture(texture->GetTarget(), texture->get());
        textures.push_back(texture);
        
        auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
        vbo->BindVBO();
        auto data = renderedObjs[0]->GetVertices();
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

void OGLModelMatrix::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glm::vec4 bgColor(0.0f, 0.2f, 0.0f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
    
    auto rotZ = glm::rotate(glm::mat4(1.0f), glm::radians((float)rZ), glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotX = glm::rotate(glm::mat4(1.0f), glm::radians((float)rX), glm::vec3(1.0f, 0.0f, 0.0f));
    auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians((float)rY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP = projection *
    glm::translate(glm::mat4(1.0f), tVector) *
    rotZ * rotX * rotY;
    
    GLint mvpLocation = glGetUniformLocation(programs[0]->get(), "MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
