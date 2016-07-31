//
//  OGLImage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>

class OGLImage
{
    
public:
    OGLImage() {}
    virtual ~OGLImage() {}
    
    virtual GLuint LoadImageFile(std::string imgPath) = 0;
};
/* OGLImage_hpp */
