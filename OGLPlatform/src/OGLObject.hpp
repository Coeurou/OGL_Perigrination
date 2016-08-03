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
	virtual void Render(double time) = 0;

    void ChangeShader(const std::pair<SHADER, std::string>& pair);
	const OGLProgram* GetProgram() const { return &program; }
	const OGLVertexArray* GetVAO() const { return &vao; }

	float GetWidth() const { return width; }
	float GetHeight() const { return height; }
	float GetDepth() const { return depth; }
	void SetWidth(float val) { width = val; }
	void SetHeight(float val) { height = val; }
	void SetDepth(float val) { depth = val; }

protected:
	OGLProgram* GetProgram() { return &program; }
	OGLVertexArray* GetVAO() { return &vao; }
	std::vector<std::shared_ptr<OGLVertexBuffer>>& GetVBOS() { return vbos; }
	std::map<SHADER, std::string>& GetShadersSource()  { return shadersSource; }

	bool InitShaders();
	float width;
	float height;
	float depth;

private:
	OGLProgram program;
	OGLVertexArray vao;
    std::vector<std::shared_ptr<OGLVertexBuffer>> vbos;
	std::map<SHADER, std::string> shadersSource;
};
/* OGLObject_hpp */
