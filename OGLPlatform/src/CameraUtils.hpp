//
//  CameraUtils.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Camera.hpp"
#include "Vertex.hpp"
#include <array>
#include <vector>

class CameraUtils
{
    
public:
    CameraUtils(gs::Camera* cam);
    ~CameraUtils();
    
    std::array<glm::vec4, 6> GetFrustumPlanes();
    
private:    
    gs::Camera* camera;
};

/* CameraUtils_hpp */
