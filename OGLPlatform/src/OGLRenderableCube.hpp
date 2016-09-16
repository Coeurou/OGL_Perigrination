//
//  OGLRenderableCube.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLRenderableObject.hpp"
#include "OGLCube.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

class OGLRenderableCube : public gs::OGLRenderableObject
{
public:
    OGLRenderableCube();
    ~OGLRenderableCube();
    
    bool Load(const std::string& meshFilename) override;
    void Render(gs::Program* program) override;
    void Render(gs::Program * program, int nbInstances) override;
    void SetSize(float size) override { cube.SetSize(size); }
    
private:
    OGLCube cube;
    gs::VertexArray vao;
    gs::VertexBuffer vbo;
};/* OGLRenderableCube_hpp */
