//
//  Texture.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include "ImageFactory.hpp"
#include "OGLResource.hpp"

enum class LIGHT_CONTRIBUTION
{
	AMBIENT = 0,
	DIFFUSE,
	SPECULAR,
	EMISSIVE,
	NORMAL
};

namespace gs
{
    class Texture : public OGLResource
    {
    public:
        Texture(IMAGE_TYPE thirdPartyType);
        ~Texture();
        
        bool LoadTexture(const std::string& imgFilename);
		bool LoadCubemap(const std::initializer_list<std::string>& imgFilenames);
        void ChangeParameter(GLenum parameter, GLint value) const;
        void ChangeParameter(GLenum parameter, GLfloat value) const;
        void BindTexture(GLenum textureUnit) const;
        
        GLuint get() const { return textureID; }

        GLenum GetTarget() const { return target; }

        int GetNbTextureInArray() const { return nbTexturesInArray; }

		LIGHT_CONTRIBUTION GetContribution() const { return contribution; }
		void SetContribution(LIGHT_CONTRIBUTION lightProperty) { contribution = lightProperty; }

    private:
        GLenum target;
		GLuint textureID;
		LIGHT_CONTRIBUTION contribution;
        IMAGE_TYPE loadingMethod;
        int nbTexturesInArray;
    };
}
/* OGLTexture_hpp */
