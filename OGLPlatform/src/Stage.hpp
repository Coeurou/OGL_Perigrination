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
#include "EventListener.hpp"
#include "OGLObjResizable.hpp"
#include "OGLRenderableObject.hpp"
#include "Program.hpp"
#include "Sampler.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

namespace gs
{    
    class Stage : public gs::IEventListener
    {
    public:
        Stage();
        virtual ~Stage();
        
        virtual bool Init(int windowWidth, int windowHeight);
        virtual void Render(double time) {}
		virtual void OnEvent(Event e) override {}

    protected:
        virtual bool InitGUI() { return true; };
        
		std::vector<std::unique_ptr<OGLObjResizable>> resizableObjs;
		std::vector<std::unique_ptr<OGLRenderableObject>> renderedObjs;
        std::vector<std::unique_ptr<VertexArray>> vaos;
        std::vector<std::unique_ptr<VertexBuffer>> vbos;
        std::vector<std::unique_ptr<Shader>> shaders;
        std::vector<std::shared_ptr<Program>> programs;
        std::vector<std::unique_ptr<Texture>> textures;
        std::vector<std::unique_ptr<Sampler>> samplers;
        std::unique_ptr<OGLAntTweakBar> atbApp;
    };
}
/* Stage_hpp */
