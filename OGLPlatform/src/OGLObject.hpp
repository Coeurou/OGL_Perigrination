//
//  OGLObject.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLProgram.hpp"
#include "OGLVertexArray.hpp"
#include "OGLVertexBuffer.hpp"
#include <map>
#include <vector>
#include <memory>

class OGLObject
{
public:
    OGLObject();
    virtual ~OGLObject();

	virtual bool Init();
	virtual void Render() = 0;

	void ChangeShader(const std::pair<SHADER, std::string>& pair) { shadersSource[pair.first] = pair.second; }

protected:
	OGLProgram* GetProgram() { return &program; }
	OGLVertexArray* GetVAO() { return &vao; }
	std::vector<OGLVertexBuffer>& GetVBOS() { return vbos; }
	std::map<SHADER, std::string>& GetShadersSource()  { return shadersSource; }

	bool InitShaders();

private:
	OGLProgram program;
	OGLVertexArray vao;
	std::vector<OGLVertexBuffer> vbos;
	std::map<SHADER, std::string> shadersSource;
};
/* OGLObject_hpp */
