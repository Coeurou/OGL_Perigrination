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
	vertexShader.SetSource(shadersSource[VERTEX]);
	res = vertexShader.Compile();
	fragShader.SetSource(shadersSource[FRAGMENT]);
	res &= fragShader.Compile();

	if (shadersSource.count(TESSCTRL)) {
		OGLShader tessCtrlShader(GL_TESS_CONTROL_SHADER);
		tessCtrlShader.SetSource(shadersSource[TESSCTRL]);
		if (tessCtrlShader.Compile()) {
			program.Attach(tessCtrlShader.get());
		}
	}

	if (GetShadersSource().count(TESSEVAL)) {
		OGLShader tessEvalShader(GL_TESS_EVALUATION_SHADER);
		tessEvalShader.SetSource(shadersSource[TESSEVAL]);
		if (tessEvalShader.Compile()) {
			program.Attach(tessEvalShader.get());
		}
	}

	if (GetShadersSource().count(GEOMETRY)) {
		OGLShader geometryShader(GL_TESS_CONTROL_SHADER);
		geometryShader.SetSource(shadersSource[GEOMETRY]);
		if (geometryShader.Compile()) {
			program.Attach(geometryShader.get());
		}
	}

	if (res) {
		program.Attach(vertexShader.get());
		program.Attach(fragShader.get());
		program.Link();
		program.Use();
	}
	else {
		std::cerr << "Can't initialize OGLObject the OpenGL program will be discarded." << std::endl;
	}
	return res;
}
