#include "OGLCube.hpp"
#include <utility>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

OGLCube::OGLCube() : texture(GLI)
{
    auto vbo(GL_ARRAY_BUFFER);
    GetVBOS().push_back(std::make_shared<OGLVertexBuffer>(vbo));
}

OGLCube::~OGLCube() 
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::array<OGLVertex, 6> Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
    OGLVertex va{ a, glm::u8vec2(0,0) };
    OGLVertex vb{ b, glm::u8vec2(1,0) };
    OGLVertex vc{ c, glm::u8vec2(1,1) };
    OGLVertex vd{ d, glm::u8vec2(0,1) };
    std::array<OGLVertex, 6> res = { va, vb, vc,  va, vc, vd };
	return res;
}

bool OGLCube::Init()
{
	GetVAO()->Bind();
	ChangeShader(std::pair<SHADER,std::string>(VERTEX, "simpleCube.vert"));
	ChangeShader(std::pair<SHADER, std::string>(FRAGMENT, "simpleCube.frag"));
	bool res = OGLObject::Init();

    res &= texture.LoadTexture("Paper_Crumbled.dds");
    
    GetVBOS()[0]->Bind();
	ComputeGeometry(1, 1, 1, glm::vec3(0,0,0.5f));
    GLsizei bufferSize = (GLsizei)(geometry.size() * sizeof(OGLVertex));
    glBufferData(GL_ARRAY_BUFFER, bufferSize, geometry.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(OGLVertex), (void*)offsetof(OGLVertex, texCoords));
    
    texture.Bind(GL_TEXTURE0);
	return res;
}

void OGLCube::Render(double time)
{
    float angle = (float)time * 45.0f; // 45° per second
    glm::mat4 rotationMat = glm::perspective(45.0f, 1024.0f / 720.0f, 0.1f, 100.f) *
                            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f,0.0f));
    GLint rotationLocation = glGetUniformLocation(GetProgram()->get(), "rotationMatrix");
    glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, glm::value_ptr(rotationMat));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void OGLCube::ComputeGeometry(float width, float height, float depth, glm::vec3 origin)
{
	float halfW = width * 0.5f;
	float halfH = height * 0.5f;
	float halfD = depth * 0.5f;

    std::array<glm::vec3, 8> vertices = {
        glm::vec3(origin.x - halfW, origin.y - halfH, origin.z + halfD),
		glm::vec3(origin.x + halfW, origin.y - halfH, origin.z + halfD),
		glm::vec3(origin.x + halfW, origin.y + halfH, origin.z + halfD),
		glm::vec3(origin.x - halfW, origin.y + halfH, origin.z + halfD),
		glm::vec3(origin.x - halfW, origin.y - halfH, origin.z - halfD),
		glm::vec3(origin.x - halfW, origin.y + halfH, origin.z - halfD),
		glm::vec3(origin.x + halfW, origin.y + halfH, origin.z - halfD),
		glm::vec3(origin.x + halfW, origin.y - halfH, origin.z - halfD)
	};
		
	auto front = Quad(vertices[0], vertices[1], vertices[2], vertices[3]);
	auto back = Quad(vertices[4], vertices[5], vertices[6], vertices[7]);
	auto left = Quad(vertices[0], vertices[4], vertices[5], vertices[3]);
	auto right = Quad(vertices[1], vertices[2], vertices[6], vertices[7]);
	auto top = Quad(vertices[2], vertices[3], vertices[5], vertices[6]);
	auto bottom = Quad(vertices[0], vertices[4], vertices[7], vertices[1]);

	std::vector<OGLVertex> cubeGeometry;
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(front), std::end(front));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(back), std::end(back));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(left), std::end(left));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(right), std::end(right));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(top), std::end(top));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(bottom), std::end(bottom));

	geometry = cubeGeometry;
}
