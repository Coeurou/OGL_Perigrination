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
#include "Camera.hpp"

class OGLCameraStage : public gs::Stage
{
	const int NB_CUBE = 5;

    gs::Camera camera;
	GLint mvpLocationTex = 0;
	GLint mvpLocationBlend = 0;
	GLint uColorLocation = 0;
	std::vector<glm::vec4> blendColors = std::vector<glm::vec4>(NB_CUBE);
    
public:
    OGLCameraStage();
    ~OGLCameraStage();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI() override;
};
/* OGLCameraStage_hpp */
