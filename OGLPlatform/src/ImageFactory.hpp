//
//  ImageFactory.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Uncopyable.hpp"
#include <memory>

namespace gs
{
    class Image;
}

enum IMAGE_TYPE
{
    GLI = 0
};

namespace gs
{
    class ImageFactory : public Uncopyable
    {
    public:
        static std::unique_ptr<Image> CreateImage(IMAGE_TYPE key);
        
    private:
        ImageFactory() {}
        ~ImageFactory() {}
    };
}
/* ImageFactory_hpp */
