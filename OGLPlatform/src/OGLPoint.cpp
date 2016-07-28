#include "OGLPoint.hpp"
#include <iostream>


OGLPoint::OGLPoint()
{
	GetShadersSource()[VERTEX] = "firstPoint.vert";
	GetShadersSource()[FRAGMENT] = "firstPoint.vert";
}


OGLPoint::~OGLPoint()
{
}

bool OGLPoint::Init()
{
	GetVAO()->Bind();	
	return OGLObject::Init();
}

void OGLPoint::Render()
{

}