//
//  OGLInstancedDraw.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "Stage.hpp"

class OGLInstancedDraw : public gs::Stage
{
    
public:
    OGLInstancedDraw();
    ~OGLInstancedDraw();
    
    bool Init(int windowWidth, int windowHeight);
    void Render(double time);
    
private:
    bool InitGUI();
};
/* OGLInstancedDraw_hpp */
