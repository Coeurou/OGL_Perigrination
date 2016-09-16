//
//  OGLRenderableObject.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLResource.hpp"
#include <string>

namespace gs
{
    class Program;
    
    class OGLRenderableObject : public OGLResource
    {
    public:
        OGLRenderableObject() {}
        virtual ~OGLRenderableObject() {}
        
        virtual bool Load(const std::string& meshFilename) { return false; };
        virtual void Render(Program* program) = 0;
        virtual void Render(Program* program, int nbInstances) = 0;
        virtual OGLRenderableObject* GetChild(int index) { return nullptr; }
        virtual void SetSize(float size) {}
    };
}
/* OGLRenderableObject_hpp */
