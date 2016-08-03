#include "OGLQuad.hpp"
#include "OGLUtils.hpp"
#include <utility>

OGLQuad::OGLQuad(std::string texImgFilename)
	: texture(GLI), imgFilename(texImgFilename)
{
    auto vbo(GL_ARRAY_BUFFER);
    GetVBOS().push_back(std::make_shared<OGLVertexBuffer>(vbo));
}

OGLQuad::~OGLQuad() 
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool OGLQuad::Init()
{
	GetVAO()->Bind();
	ChangeShader(std::pair<SHADER,std::string>(VERTEX, "simpleQuad.vert"));
	ChangeShader(std::pair<SHADER, std::string>(FRAGMENT, "simpleQuad.frag"));
	bool res = OGLObject::Init();

    res &= texture.LoadTexture(imgFilename);
    
    GetVBOS()[0]->Bind();
	ComputeGeometry( glm::vec3(0,0,0.5f));
    GLsizei bufferSize = (GLsizei)(geometry.size() * sizeof(OGLVertex));
    glBufferData(GL_ARRAY_BUFFER, bufferSize, geometry.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, texCoords));
    
	return res;
}

void OGLQuad::Render(double time)
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void OGLQuad::ComputeGeometry(glm::vec3 origin)
{
	float halfW = width * 0.5f;
	float halfH = height * 0.5f;
	float halfD = depth * 0.5f;

	std::array<OGLVertex, 4> vertices =
	{
		OGLVertex{ glm::vec3(origin.x + halfW, origin.y - halfH, origin.z + halfD), glm::u8vec2((int)width,0) },
		OGLVertex{ glm::vec3(origin.x + halfW, origin.y + halfH, origin.z + halfD), glm::u8vec2((int)width,(int)height) },
		OGLVertex{ glm::vec3(origin.x - halfW, origin.y - halfH, origin.z + halfD), glm::u8vec2(0,0) },
		OGLVertex{ glm::vec3(origin.x - halfW, origin.y + halfH, origin.z + halfD), glm::u8vec2(0,(int)height) }
	};

	geometry.insert(std::end(geometry), std::begin(vertices), std::end(vertices));
}
