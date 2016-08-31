//
//  OGLRenderableObject.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLResource.hpp"

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
};
}
/* OGLRenderableObject */
