//
//  OGLRenderableQuad.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLRenderableObject.hpp"
#include "OGLQuad.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

class OGLRenderableQuad : public gs::OGLRenderableObject
{
public:
    OGLRenderableQuad();
    ~OGLRenderableQuad();
    
    bool Load(const std::string& meshFilename) override;
    void Render() override;
    void Render(int nbInstances) override;
    void SetSize(float size) override { quad.SetSize(size); }
    
private:
    OGLQuad quad;
    gs::VertexArray vao;
    gs::VertexBuffer vbo;
};/* OGLRenderableQuad_hpp */
