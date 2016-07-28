//
//  OGLShader.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>

enum SHADER
{
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	TESSCTRL,
	TESSEVAL
};

class OGLShader
{
public:
	OGLShader(GLenum shaderType);
	~OGLShader();

	const GLuint get() const { return shaderID; }
	bool SetSource(const std::string& shaderFilePath);
	bool Compile();

	bool operator<(const OGLShader& rhs) const
	{
		return shaderID < rhs.shaderID;
	}

private:
	GLuint shaderID;
};
/* OGLShader_hpp */
