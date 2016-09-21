//
//  OGLDepthPicking.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include "OGLRenderableCube.hpp"
#include "OGLRenderableQuad.hpp"

class OGLDepthPicking : public gs::Stage
{
    gs::Camera camera;
    std::vector<std::shared_ptr<OGLRenderableCube>> cubes;
    OGLRenderableQuad ground;
    
public:
    OGLDepthPicking();
    ~OGLDepthPicking();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI() override;
};
/* OGLDepthPicking_hpp */
