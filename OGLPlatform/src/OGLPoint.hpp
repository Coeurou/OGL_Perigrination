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
    
    virtual void SetWidth(float newWidth)
    {
        width = newWidth;
        glPointSize(width);
    }
    virtual void SetHeight(float newHeight)
    {
        width = newHeight;
        glPointSize(width);
    }

    virtual void SetDepth(float newDepth)
    {
        width = newDepth;
        glPointSize(width);
    }

};

