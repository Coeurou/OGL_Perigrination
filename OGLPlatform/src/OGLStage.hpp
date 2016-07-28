//
//  OGLStage.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLObject.hpp"
#include "OGLAntTweakBar.hpp"
#include <vector>
#include <memory>

class OGLStage
{
public:
    OGLStage();
    virtual ~OGLStage();

	virtual bool Init(int windowWidth, int windowHeight);
	virtual void Render(double time) {}

protected:
	std::vector<OGLObject>& GetRenderedObjs() { return renderedObjs; }
	OGLAntTweakBar* const GetTweakBar() { return atbApp.get(); }

private:
    std::vector<OGLObject> renderedObjs;
	std::unique_ptr<OGLAntTweakBar> atbApp;
};
/* OGLStage_hpp */
