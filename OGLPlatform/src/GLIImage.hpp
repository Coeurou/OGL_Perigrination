//
//  GLIImage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once
#include "Image.hpp"

class GLIImage : public gs::Image
{
    
public:
    GLIImage();
    ~GLIImage();
    
    bool LoadImageFile(const std::string& imgPath) override;
	bool LoadCubemapFile(const std::vector<std::string>& imgPaths) override;
}; /* GLIImage_hpp */
