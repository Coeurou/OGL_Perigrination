//
//  GLIImage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once
#include "OGLImage.hpp"

class GLIImage : public OGLImage
{
    
public:
    GLIImage();
    ~GLIImage();
    
    bool LoadImageFile(const std::string& imgPath);
}; /* GLIImage_hpp */
