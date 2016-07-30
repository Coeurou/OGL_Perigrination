#include "OGLTriangle.hpp"
#include <iostream>


OGLTriangle::OGLTriangle()
{
	GetShadersSource()[VERTEX] = "simpleTriangle.vert";
	GetShadersSource()[FRAGMENT] = "simpleTriangle.frag";
}


OGLTriangle::~OGLTriangle()
{
}

bool OGLTriangle::Init()
{	
	GetVAO()->Bind();		
	return OGLObject::Init();
}

void OGLTriangle::Render(double time)
{

}