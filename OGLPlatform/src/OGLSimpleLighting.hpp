//
//  OGLCameraStage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 15/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class OGLSimpleLighting : public gs::Stage
{
	const int NB_CUBE = 5;

    gs::Camera camera;
	GLint mvpLocation = 0;
	GLint normalMatrixLocation = 0;
	gs::DirectionnalLight light;
    
public:
	OGLSimpleLighting();
    ~OGLSimpleLighting();
    
    bool Init(int windowWidth, int windowHeight);
    void Render(double time);
    
private:
    bool InitGUI();
};
/* OGLCameraStage_hpp */
