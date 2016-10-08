//
//  OGLImageFactory.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "ImageFactory.hpp"
#include "Image.hpp"
#include "GLIImage.hpp"

namespace gs
{
    std::unique_ptr<gs::Image> ImageFactory::CreateImage(IMAGE_TYPE key)
    {
        switch (key) {
            case GLI:
                return std::make_unique<GLIImage>();
                break;
            default:
                return nullptr;
                break;
        }
    }
}