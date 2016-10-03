//
//  OGLContext.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

namespace gs
{
    struct ContextParams
    {
        int minorVersion;
        int majorVersion;
        int msaaSamples;
        int windowHeight;
        int windowWidth;
        
        ContextParams() : minorVersion(1), majorVersion(4), msaaSamples(4),
        windowWidth(1024), windowHeight(720) {}
    };
    
    class Context
    {
        
    public:
        Context() {}
        Context(ContextParams cParams) : params(cParams) {}
        virtual ~Context() {}
        
		virtual bool InitContext() { return true; }
        ContextParams GetContextParams() const { return params; }
        void SetContextParams(ContextParams newParams) { params = newParams; }
        
    protected:
        ContextParams params;
    };
}
/* OGLContext_hpp */
