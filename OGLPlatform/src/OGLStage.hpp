//
//  OGLStage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLObjResizable.hpp"
#include "OGLShader.hpp"
#include "OGLProgram.hpp"
#include "OGLVertexArray.hpp"
#include "OGLVertexBuffer.hpp"
#include "OGLTexture.hpp"
#include "OGLAntTweakBar.hpp"
#include <vector>
#include <memory>

class OGLStage
{
public:
    OGLStage();
    virtual ~OGLStage();

	virtual bool Init(int windowWidth, int windowHeight);
	virtual void Render(double time) {}

protected:
    virtual bool InitGUI() { return true; };
    
    std::vector<std::unique_ptr<OGLObjResizable>> renderedObjs;
    std::vector<std::shared_ptr<OGLVertexArray>> vaos;
    std::vector<std::shared_ptr<OGLVertexBuffer>> vbos;
    std::vector<std::shared_ptr<OGLShader>> shaders;
    std::vector<std::shared_ptr<OGLProgram>> programs;
    std::vector<std::shared_ptr<OGLTexture>> textures;
	std::unique_ptr<OGLAntTweakBar> atbApp;
};
/* OGLStage_hpp */
