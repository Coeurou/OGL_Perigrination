//
//  Image.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

namespace gs
{
    class Image
    {
    public:
        Image() {}
        virtual ~Image() {}
        
		virtual bool LoadImageFile(const std::string& imgPath) = 0;
		virtual bool LoadCubemapFile(const std::vector<std::string>& imgPath) = 0;
        GLuint GetTextureID() const { return textureID; }
        GLenum GetTarget() const { return target; }
        int GetNbFaces() const { return nbFaces; }
        
    protected:
        GLuint textureID;
        GLenum target;
        int nbFaces;
    };
}
/* Image_hpp */
