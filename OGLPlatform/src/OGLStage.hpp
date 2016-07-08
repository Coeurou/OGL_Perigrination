//
//  OGLStage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <vector>
#include "OGLObject.hpp"

class OGLStage
{
public:
    OGLStage();
    ~OGLStage();
private:
    std::vector<OGLObject> renderedObjs;
};
/* OGLStage_hpp */
