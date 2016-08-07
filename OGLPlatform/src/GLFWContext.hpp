//
//  GLFWContext.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 07/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Context.hpp"

class GLFWContext : public gs::Context
{
    
public:
    GLFWContext();
    ~GLFWContext();
    
    bool InitContext();
};
/* GLFWContext_hpp */
