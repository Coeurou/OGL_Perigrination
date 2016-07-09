#include "OGLPoint.hpp"


OGLPoint::OGLPoint()
{
}


OGLPoint::~OGLPoint()
{
}

bool OGLPoint::Init()
{
	bool res = false;
	GetVAO()->Bind();

	OGLShader vertexShader(GL_VERTEX_SHADER);
	OGLShader fragShader(GL_FRAGMENT_SHADER);
	vertexShader.SetSource("firstPoint.vert");
	res = vertexShader.Compile();
	fragShader.SetSource("firstPoint.frag");
	res &= fragShader.Compile();
	auto prog = GetProgram();
	prog->Attach(vertexShader.get());
	prog->Attach(fragShader.get());
	prog->Link();
	prog->Use();

	return res;
}

void OGLPoint::Render()
{

}