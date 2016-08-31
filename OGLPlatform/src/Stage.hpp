//
//  Stage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLAntTweakBar.hpp"
#include <vector>
#include <memory>
#include "OGLObjResizable.hpp"
#include "OGLRenderableObject.hpp"

namespace gs
{
    class Program;
    class Sampler;
    class Shader;
    class Texture;
    class VertexArray;
    class VertexBuffer;
    
    class Stage
    {
    public:
        Stage();
        virtual ~Stage();
        
        virtual bool Init(int windowWidth, int windowHeight);
        virtual void Render(double time) {}
        
    protected:
        virtual bool InitGUI() { return true; };
        
		std::vector<std::unique_ptr<OGLObjResizable>> resizableObjs;
		std::vector<std::unique_ptr<OGLRenderableObject>> renderedObjs;
        std::vector<std::shared_ptr<VertexArray>> vaos;
        std::vector<std::shared_ptr<VertexBuffer>> vbos;
        std::vector<std::shared_ptr<Shader>> shaders;
        std::vector<std::shared_ptr<Program>> programs;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Sampler>> samplers;
        std::unique_ptr<OGLAntTweakBar> atbApp;
    };
}
/* Stage_hpp */
