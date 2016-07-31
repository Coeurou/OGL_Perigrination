//
//  OGLImageFactory.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Uncopyable.hpp"
#include "OGLImage.hpp"
#include <memory>

enum IMAGE_TYPE
{
    GLI = 0
};

class OGLImageFactory : public Uncopyable
{
public:
    static std::shared_ptr<OGLImage> CreateImage(IMAGE_TYPE key);
    
private:
    OGLImageFactory() {}
    ~OGLImageFactory() {}
};
/* OGLImageFactory_hpp */
