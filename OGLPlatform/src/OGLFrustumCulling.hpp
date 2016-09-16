//
//  OGLFrustumCulling.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include "Camera.hpp"
#include "Query.hpp"
#include "CameraUtils.hpp"
#include "OGLRenderableQuad.hpp"
#include <glm/glm.hpp>

class OGLFrustumCulling : public gs::Stage
{
    CameraUtils frustumComputer;
    gs::Camera camera;
    gs::Camera cameraExternal;
    bool useExternalCamera = false;
    const int NUM_VERTICESX = 500;
    const int NUM_VERTICESZ = 500;
    const int HALF_SIZEX = 100;
    const int HALF_SIZEZ = 100;
    int totalVertices = 0;
    float waveAmplitude = 0.5f;
    gs::Query primitiveQuery;
    OGLRenderableQuad ground;
    
public:
    OGLFrustumCulling();
    ~OGLFrustumCulling();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI() override;
};
/* OGLFrustumCulling_hpp */
