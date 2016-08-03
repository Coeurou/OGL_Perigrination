#include "FactoryStage.hpp"
#include "OGLPointSize.hpp"
#include "OGLSimpleTriangle.hpp"
#include "OGLAnimatedTriangle.hpp"
#include "OGLSimpleCube.hpp"
#include "OGLMipmaps.hpp"
#include "OGLArrayTexture.hpp"

std::shared_ptr<OGLStage> FactoryStage::CreateStage(STAGES key)
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
	case ARRAY_TEXTURE:
		return std::make_shared<OGLArrayTexture>();
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
