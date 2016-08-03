//
//  OGLObject.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLObject.hpp"
#include <iostream>

OGLObject::OGLObject() : width(1), height(1), depth(1)
{}

OGLObject::~OGLObject()
{}

void OGLObject::ChangeShader(const std::pair<SHADER, std::string> &pair)
{
     shadersSource[pair.first] = pair.second;
}

bool OGLObject::Init()
{
	return InitShaders();
}

bool OGLObject::InitShaders()
{
	bool res = true;

	OGLShader vertexShader(GL_VERTEX_SHADER);
	OGLShader fragShader(GL_FRAGMENT_SHADER);
	vertexShader.SetSource(shadersSource[VERTEX]);
    if (!vertexShader.Compile()) {
        std::cerr << "Vertex Shader compilation failed file: " << shadersSource[VERTEX] << std::endl;
        res = false;
    }
    
    
	fragShader.SetSource(shadersSource[FRAGMENT]);
    if (!fragShader.Compile()) {
        std::cerr << "Fragment Shader compilation failed file: " << shadersSource[FRAGMENT] << std::endl;
        res = false;
    }

	if (shadersSource.count(TESSCTRL)) {
		OGLShader tessCtrlShader(GL_TESS_CONTROL_SHADER);
		tessCtrlShader.SetSource(shadersSource[TESSCTRL]);
		if (tessCtrlShader.Compile()) {
			program.Attach(tessCtrlShader.get());
		}
        else {
            std::cerr << "Tesselaton control Shader compilation failed file: " << shadersSource[TESSCTRL] << std::endl;
        }
	}

	if (GetShadersSource().count(TESSEVAL)) {
		OGLShader tessEvalShader(GL_TESS_EVALUATION_SHADER);
		tessEvalShader.SetSource(shadersSource[TESSEVAL]);
		if (tessEvalShader.Compile()) {
			program.Attach(tessEvalShader.get());
		}
        else {
            std::cerr << "Tesselaton evaluation Shader compilation failed file: " << shadersSource[TESSEVAL] << std::endl;
        }
	}

	if (GetShadersSource().count(GEOMETRY)) {
		OGLShader geometryShader(GL_TESS_CONTROL_SHADER);
		geometryShader.SetSource(shadersSource[GEOMETRY]);
		if (geometryShader.Compile()) {
			program.Attach(geometryShader.get());
		}
        else {
            std::cerr << "Geometry Shader compilation failed file: " << shadersSource[GEOMETRY] << std::endl;
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
