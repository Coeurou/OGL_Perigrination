#include "OGLCube.hpp"


OGLCube::OGLCube()
{
	GetVBOS().push_back(OGLVertexBuffer(GL_ARRAY_BUFFER));
}

OGLCube::~OGLCube() 
{}

std::array<glm::vec3, 6> Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
	std::array<glm::vec3, 6> res = {a, b, c, a, c, d};
	return res;
}

bool OGLCube::Init()
{
	GetVAO()->Bind();
	auto shaders = GetShadersSource();
	ChangeShader(std::pair<SHADER,std::string>(VERTEX, "simpleCube.vert"));
	ChangeShader(std::pair<SHADER, std::string>(FRAGMENT, "simpleCube.frag"));
	bool res = OGLObject::Init();

	GetVBOS()[0].Bind();
	ComputeGeometry(1, 1, 0.25f, glm::vec3(0,0,0.5f));
	glBufferData(GL_ARRAY_BUFFER, /*SIZE::CUBE * sizeof(glm::vec3)*/sizeof(geometry), &geometry[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return res;
}

void OGLCube::Render()
{
	GetProgram()->Use();

	glEnableVertexAttribArray(0);
	GetVBOS()[0].Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, SIZE::CUBE);
	glDisableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OGLCube::ComputeGeometry(float width, float height, float depth, glm::vec3 origin)
{
	float halfW = width * 0.5f;
	float halfH = height * 0.5f;
	float halfD = depth * 0.5f;

	std::array<glm::vec3, 8> vertices = {
		glm::vec3(origin.x - halfW, origin.y - halfH, origin.z - halfD),
		glm::vec3(origin.x + halfW, origin.y - halfH, origin.z - halfD),
		glm::vec3(origin.x + halfW, origin.y + halfH, origin.z - halfD),
		glm::vec3(origin.x - halfW, origin.y + halfH, origin.z - halfD),
		glm::vec3(origin.x - halfW, origin.y - halfH, origin.z + halfD),
		glm::vec3(origin.x - halfW, origin.y + halfH, origin.z + halfD),
		glm::vec3(origin.x + halfW, origin.y + halfH, origin.z + halfD),
		glm::vec3(origin.x + halfW, origin.y - halfH, origin.z + halfD)
	};
		
	auto front = Quad(vertices[0], vertices[1], vertices[2], vertices[3]);
	auto back = Quad(vertices[4], vertices[5], vertices[6], vertices[7]);
	auto left = Quad(vertices[0], vertices[3], vertices[4], vertices[5]);
	auto right = Quad(vertices[1], vertices[2], vertices[6], vertices[7]);
	auto top = Quad(vertices[2], vertices[3], vertices[5], vertices[6]);
	auto bottom = Quad(vertices[0], vertices[1], vertices[4], vertices[7]);

	std::vector<glm::vec3> cubeGeometry;
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(front), std::end(front));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(back), std::end(back));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(left), std::end(left));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(right), std::end(right));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(top), std::end(top));
	cubeGeometry.insert(std::end(cubeGeometry), std::begin(bottom), std::end(bottom));

	geometry = cubeGeometry;
}
