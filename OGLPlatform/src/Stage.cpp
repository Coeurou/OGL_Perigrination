//
//  Stage.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Stage.hpp"
#include <GL/glew.h>
#include <iostream>
#include "Texture.hpp"
#include "VertexBuffer.hpp"

namespace gs
{
    Stage::Stage() : atbApp(new OGLAntTweakBar)
    {}
    
    Stage::~Stage()
    {
        GLenum target = 0;
        for (auto vbo : vbos) {
            if (target != vbo->GetTarget()) {
                target = vbo->GetTarget();
                glBindBuffer(target, 0);
            }
        }
        target = 0;
        for (auto texture : textures) {
            if (target != texture->GetTarget()) {
                target = texture->GetTarget();
                glBindTexture(target, 0);
            }
        }
        glUseProgram(0);
        glBindVertexArray(0);
    }
    
    bool Stage::Init(int windowWidth, int windowHeight)
    {
        bool res = atbApp->InitAntTweakBar("TweakBar", windowWidth, windowHeight);
        if (!res) {
            std::cerr << "Error in AntTweakBar initialization" << std::endl;
        }
        return res;
    }
}