#include "FactoryStage.hpp"
#include "OGLPointSize.hpp"
#include "OGLSimpleTriangle.hpp"
#include "OGLAnimatedTriangle.hpp"
#include "OGLSimpleCube.hpp"
#include "OGLMipmaps.hpp"
#include "OGLMixTexture.hpp"
#include "OGLRipplePlane.hpp"
#include "OGLGeometryShader.hpp"
#include "OGLModelMatrix.hpp"
#include "OGLInstancedDraw.hpp"
#include "OGLCameraStage.hpp"
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
	case RIPPLE:
		return std::make_shared<OGLRipplePlane>();
		break;
    case GEOMETRYSHADER:
        return std::make_shared<OGLGeometryShader>();
        break;
    case MODELMAT:
        return std::make_shared<OGLModelMatrix>();
        break;
    case INSTANCEDRAW:
        return std::make_shared<OGLInstancedDraw>();
        break;
    case CAMERA:
        return std::make_shared<OGLCameraStage>();
        break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
