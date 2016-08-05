//
//  OGLStage.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLStage.hpp"
#include <iostream>

OGLStage::OGLStage() : atbApp(new OGLAntTweakBar)
{}

OGLStage::~OGLStage()
{
    GLenum target = 0;
    for (auto vbo : vbos) {
        if (target != vbo->getTarget()) {
            target = vbo->getTarget();
            glBindBuffer(target, 0);
        }
    }
    target = 0;
    for (auto texture : textures) {
        if (target != texture->getTarget()) {
            target = texture->getTarget();
            glBindTexture(target, 0);
        }
    }
    glUseProgram(0);
    glBindVertexArray(0);
}

bool OGLStage::Init(int windowWidth, int windowHeight)
{
	bool res = atbApp->InitAntTweakBar("TweakBar", windowWidth, windowHeight);
	if (!res) {
		std::cerr << "Error in AntTweakBar initialization" << std::endl;
	}
	return res;
}
