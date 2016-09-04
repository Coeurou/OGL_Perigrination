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
#include "OGLLight.hpp"
#include "Camera.hpp"

class OGLSimpleLighting : public gs::Stage
{
	const int NB_CUBE = 5;

    gs::Camera camera;
	GLint mvpLocation = 0;
	GLint normalMatrixLocation = 0;
	GLint lightDirLoc = 0;
	GLint specularLoc = 0;
	GLint diffuseLoc = 0;
	GLint ambientLoc = 0;
	OGLLight lightObj;
	gs::DirectionnalLight light;

public:
	OGLSimpleLighting();
    ~OGLSimpleLighting();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI();
};
/* OGLCameraStage_hpp */
