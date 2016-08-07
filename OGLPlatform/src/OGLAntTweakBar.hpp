//
//  OGLAntTweakBar.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Uncopyable.hpp"
#include <AntTweakBar.h>
#include <vector>

class OGLAntTweakBar : public Uncopyable
{
    
public:
    OGLAntTweakBar();
    ~OGLAntTweakBar();
    
    bool InitAntTweakBar(const std::string& barTitle, int width, int height);
    void AddTweakBar(TwBar* bar);
    TwBar* const GetBarByIndex(int index) const { return tweakBars[index]; }
    
private:
    std::vector<TwBar*> tweakBars;
};