//
//  Texture.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Texture.hpp"
#include "Image.hpp"
#include "OGLApplicationConstants.hpp"
#include <iostream>
#include <vector>

namespace gs
{
    Texture::Texture(IMAGE_TYPE thirdPartyType) : loadingMethod(thirdPartyType)
    {}
    
    Texture::~Texture()
    {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    
    bool Texture::LoadTexture(std::string imgFilename)
    {
        imgFilename = TexturesPath + "/" + imgFilename;
        
        auto texImg = ImageFactory::CreateImage(loadingMethod);
        texImg->LoadImageFile(imgFilename);
        
        textureID = texImg->GetTextureID();
        target = texImg->GetTarget();
        nbTexturesInArray = texImg->GetNbFaces();
        
        bool res = (textureID != 0);
        if (res) {
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            std::cerr << "Texture " << imgFilename << " loading failed" << std::endl;
        }
        return res;
    }

	bool Texture::LoadCubemap(const std::initializer_list<std::string>& imgFilenames)
	{
		bool res = true;
		std::vector<std::string> imgPaths;
		for (auto& imgFilename : imgFilenames) {
			imgPaths.push_back(TexturesPath + "/" + imgFilename);
		}

		auto texImg = ImageFactory::CreateImage(loadingMethod);
		texImg->LoadCubemapFile(imgPaths);

		textureID = texImg->GetTextureID();
		target = texImg->GetTarget();
		nbTexturesInArray = texImg->GetNbFaces();

		res &= (textureID != 0);
		if (res) {
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else {
			std::cerr << "Cubemap " << *imgFilenames.begin() << " loading failed" << std::endl;
		}
		return res;
	}
    
    void Texture::ChangeParameter(GLenum parameter, GLint value) const
    {
        glTexParameteri(target, parameter, value);
    }
    
    void Texture::ChangeParameter(GLenum parameter, GLfloat value) const
    {
        glTexParameterf(target, parameter, value);
    }
    void Texture::BindTexture(GLenum textureUnit) const
    {
        glActiveTexture(textureUnit);
        glBindTexture(target, textureID);
    }
}