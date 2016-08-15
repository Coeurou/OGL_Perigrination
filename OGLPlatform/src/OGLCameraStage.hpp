//
//  OGLCameraStage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 15/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class OGLCameraStage : public gs::Stage
{
    gs::Camera camera;
    GLint mvpLocation = 0;
    
public:
    OGLCameraStage();
    ~OGLCameraStage();
    
    bool Init(int windowWidth, int windowHeight);
    void Render(double time);
    
private:
    bool InitGUI();
};
/* OGLCameraStage_hpp */
