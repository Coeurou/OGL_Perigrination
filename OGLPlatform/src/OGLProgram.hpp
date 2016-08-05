//
//  OGLProgram.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <set>
#include "OGLShader.hpp"
#include "OGLResource.hpp"

class OGLProgram : public OGLResource
{
public:
	OGLProgram();
	~OGLProgram();
	const GLuint get() const { return programID; };
	void Attach(GLuint shader);
	void Detach(GLuint shader);
	void Link();
	void Use() const;

private:
	GLuint programID;
	std::set<GLuint> shaders;
};
/* OGLProgram_hpp */
