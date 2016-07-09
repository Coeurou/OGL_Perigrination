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
{ }

OGLStage::~OGLStage()
{ }

bool OGLStage::Init(int windowWidth, int windowHeight)
{
	bool res = atbApp->InitAntTweakBar("TweakBar", windowWidth, windowHeight);
	if (!res)
	{
		std::cerr << "Error in AntTweakBar initialization" << std::endl;
	}
	return res;
}
