#include "FactoryStage.hpp"
#include "OGLPointSize.hpp"
#include "OGLSimpleTriangle.hpp"
#include "OGLAnimatedPyramids.hpp"
#include "OGLSimpleCube.hpp"
#include "OGLMipmaps.hpp"
#include "OGLMixTexture.hpp"
#include "OGLRipplePlane.hpp"
#include "OGLGeometryShader.hpp"
#include "OGLModelMatrix.hpp"
#include "OGLIndexedDraw.hpp"
#include "OGLCameraStage.hpp"
#include "OGLSimpleLighting.hpp"
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
	case ANIMATED_PYRAMIDS:
		return std::make_shared<OGLAnimatedPyramids>();
		break;
	case MIPMAPS:
		return std::make_shared<OGLMipmaps>();
		break;
	case MIX_TEXTURE:
		return std::make_shared<OGLMixTexture>();
		break;
	case RIPPLE:
		return std::make_shared<OGLRipplePlane>();
		break;
    case GEOMETRYSHADER:
        return std::make_shared<OGLGeometryShader>();
        break;
    case MODELMAT:
        return std::make_shared<OGLModelMatrix>();
        break;
    case INDEXEDDRAW:
        return std::make_shared<OGLIndexedDraw>();
        break;
    case CAMERA:
        return std::make_shared<OGLCameraStage>();
        break;
	case SIMPLE_LIGHT:
		return std::make_shared<OGLSimpleLighting>();
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
