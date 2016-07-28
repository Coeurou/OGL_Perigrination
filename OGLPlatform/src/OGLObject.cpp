//
//  OGLObject.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLObject.hpp"
#include <iostream>

OGLObject::OGLObject()
{}

OGLObject::~OGLObject()
{}

bool OGLObject::Init()
{
	return InitShaders();
}

bool OGLObject::InitShaders()
{
	bool res = false;

	OGLShader vertexShader(GL_VERTEX_SHADER);
	OGLShader fragShader(GL_FRAGMENT_SHADER);
	vertexShader.SetSource(GetShadersSource()[VERTEX]);
	res = vertexShader.Compile();
	fragShader.SetSource(GetShadersSource()[FRAGMENT]);
	res &= fragShader.Compile();

	if (GetShadersSource().count(TESSCTRL)) {
		OGLShader tessCtrlShader(GL_TESS_CONTROL_SHADER);
		tessCtrlShader.SetSource(GetShadersSource()[TESSCTRL]);
		if (tessCtrlShader.Compile()) {
			GetProgram()->Attach(tessCtrlShader.get());
		}
	}

	if (GetShadersSource().count(TESSEVAL)) {
		OGLShader tessEvalShader(GL_TESS_EVALUATION_SHADER);
		tessEvalShader.SetSource(GetShadersSource()[TESSEVAL]);
		if (tessEvalShader.Compile()) {
			GetProgram()->Attach(tessEvalShader.get());
		}
	}

	if (GetShadersSource().count(GEOMETRY)) {
		OGLShader geometryShader(GL_TESS_CONTROL_SHADER);
		geometryShader.SetSource(GetShadersSource()[GEOMETRY]);
		if (geometryShader.Compile()) {
			GetProgram()->Attach(geometryShader.get());
		}
	}

	if (res) {
		auto prog = GetProgram();
		prog->Attach(vertexShader.get());
		prog->Attach(fragShader.get());
		prog->Link();
		prog->Use();
	}
	else {
		std::cerr << "Can't initialize OGLObject the OpenGL program will be discarded." << std::endl;
	}
	return res;
}
