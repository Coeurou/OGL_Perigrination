//
//  OGLImageFactory.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLImageFactory.hpp"
#include "GLIImage.hpp"

std::shared_ptr<OGLImage> OGLImageFactory::CreateImage(IMAGE_TYPE key)
{
    switch (key) {
        case GLI:
            return std::make_shared<GLIImage>();
            break;
        default:
            return nullptr;
            break;
    }
}