//
//  OGLAntTweakBar.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLAntTweakBar.hpp"


OGLAntTweakBar::OGLAntTweakBar()
{}

OGLAntTweakBar::~OGLAntTweakBar()
{
    TwTerminate();
}

bool OGLAntTweakBar::InitAntTweakBar(const std::string& barTitle, int width, int height)
{
    bool res = false;
    
    res = (TwInit(TW_OPENGL_CORE, NULL) == 1);
    TwWindowSize(width, height);
    TwBar* bar;
    bar = TwNewBar(barTitle.c_str());
    AddTweakBar(bar);
    
    return res;
}

void OGLAntTweakBar::AddTweakBar(TwBar* bar)
{
    tweakBars.push_back(bar);
}