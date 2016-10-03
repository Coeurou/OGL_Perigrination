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
#include <memory>

namespace gs
{
    class Program;
    
    class OGLRenderableObject : public OGLResource
    {
    public:
        OGLRenderableObject() {}
        virtual ~OGLRenderableObject() {}
        
        virtual bool Load(const std::string& filename) { return false; };
        virtual void Render() = 0;
        virtual void Render(int nbInstances) = 0;
        virtual OGLRenderableObject* GetChild(int index) { return nullptr; }
        virtual void SetSize(float size) {}
		void SetProgram(const std::shared_ptr<Program> prog) { program = prog; }

	protected:
		std::shared_ptr<Program> program;
    };
}
/* OGLRenderableObject_hpp */
