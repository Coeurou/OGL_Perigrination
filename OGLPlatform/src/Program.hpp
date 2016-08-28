//
//  Program.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <set>
#include <map>
#include "OGLResource.hpp"

namespace gs
{
    class Program : public OGLResource
    {
    public:
        Program();
        ~Program();

		bool CreateShader(GLenum shaderType, const std::string& source);

        const GLuint get() const { return programID; };
		GLuint GetUniform(const std::string& name) const { return uniformLocations.at(name); }

        void Attach(GLuint shader);
        void Detach(GLuint shader);
        bool Link();
        void Use() const;
		bool AddUniform(const std::string& name);
        
    private:
        GLuint programID;
		std::set<GLuint> shaders;
		std::map<std::string, GLuint> uniformLocations;
    };
}
/* Program_hpp */
