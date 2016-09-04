#pragma once

#include "OGLObjResizable.hpp"
#include <GL/glew.h>

class OGLPoint : public OGLObjResizable
{
public:
    OGLPoint();
	OGLPoint(float size);
	~OGLPoint();
    
    bool InitVertices(glm::vec3 origin) override;
    
    virtual void SetWidth(float newWidth) override
    {
        width = newWidth;
        glPointSize(width);
    }
    virtual void SetHeight(float newHeight) override
    {
        width = newHeight;
        glPointSize(width);
    }

    virtual void SetDepth(float newDepth) override
    {
        width = newDepth;
        glPointSize(width);
    }

};

