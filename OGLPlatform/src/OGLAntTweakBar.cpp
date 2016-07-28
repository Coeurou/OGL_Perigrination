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
	for (auto bar : tweakBars) {
		TwDeleteBar(bar);
	}
}

bool OGLAntTweakBar::InitAntTweakBar(const std::string& barTitle, int width, int height)
{    
    TwWindowSize(width, height);
    TwBar* bar;
    bar = TwNewBar(barTitle.c_str());
	auto str = barTitle + " position='0 0'";
	TwDefine(str.c_str());

    AddTweakBar(bar);
    
    return (bar != nullptr);
}

void OGLAntTweakBar::AddTweakBar(TwBar* bar)
{
    tweakBars.push_back(bar);
}