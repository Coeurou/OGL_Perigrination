#include "FactoryStage.hpp"
#include "OGLPointSize.hpp"
#include "OGLSimpleTriangle.hpp"
#include "OGLAnimatedTriangle.hpp"
#include "OGLSimpleCube.hpp"
#include "OGLMipmaps.hpp"
#include "OGLMixTexture.hpp"
#include "Stage.hpp"


std::shared_ptr<gs::Stage> FactoryStage::CreateStage(STAGES key)
{
	switch (key)
	{
	case ALL:
		break;
	case POINT_SIZE:
		return std::make_shared<OGLPointSize>();
		break;
	case SIMPLE_TRIANGLE:
		return std::make_shared<OGLSimpleTriangle>();
		break;
	case ANIMATED_TRIANGLE:
		return std::make_shared<OGLAnimatedTriangle>();
		break;
	case SIMPLE_CUBE:
		return std::make_shared<OGLSimpleCube>();
		break;
	case MIPMAPS:
		return std::make_shared<OGLMipmaps>();
		break;
	case MIX_TEXTURE:
		return std::make_shared<OGLMixTexture>();
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
