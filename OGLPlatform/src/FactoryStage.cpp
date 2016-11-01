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
#include "OGLSkybox.hpp"
#include "OGLMaterialInstancedDraw.hpp"
#include "OGLPointLight.hpp"
#include "OGLSpotLight.hpp"
#include "OGLMultipleLights.hpp"
#include "OGLFrustumCulling.hpp"
#include "OGLDepthPicking.hpp"
#include "OGLColorPicking.hpp"
#include "OGLRayPicking.hpp"
#include "OGLRefractionReflection.hpp"
#include "Stage.hpp"


std::unique_ptr<gs::Stage> FactoryStage::CreateStage(STAGES key)
{
    switch (key)
    {
        case ALL:
            break;
        case POINT_SIZE:
            return std::make_unique<OGLPointSize>();
            break;
        case SIMPLE_TRIANGLE:
            return std::make_unique<OGLSimpleTriangle>();
            break;
        case ANIMATED_PYRAMIDS:
            return std::make_unique<OGLAnimatedPyramids>();
            break;
        case MIPMAPS:
            return std::make_unique<OGLMipmaps>();
            break;
        case MIX_TEXTURE:
            return std::make_unique<OGLMixTexture>();
            break;
        case RIPPLE:
            return std::make_unique<OGLRipplePlane>();
            break;
        case GEOMETRYSHADER:
            return std::make_unique<OGLGeometryShader>();
            break;
        case MODELMAT:
            return std::make_unique<OGLModelMatrix>();
            break;
        case INDEXEDDRAW:
            return std::make_unique<OGLIndexedDraw>();
            break;
        case CAMERA:
            return std::make_unique<OGLCameraStage>();
            break;
        case SIMPLE_LIGHT:
            return std::make_unique<OGLSimpleLighting>();
            break;
        case SKYBOX:
            return std::make_unique<OGLSkybox>();
            break;
        case MATERIAL:
            return std::make_unique<OGLMaterialInstancedDraw>();
            break;
        case POINTLIGHT:
            return std::make_unique<OGLPointLight>();
            break;
        case SPOTLIGHT:
            return std::make_unique<OGLSpotLight>();
            break;
        case MULTIPLE_LIGHTS:
            return std::make_unique<OGLMultipleLights>();
            break;
        case FRUSTUM_CULLING:
            return std::make_unique<OGLFrustumCulling>();
            break;
        case DEPTH_PICKING:
            return std::make_unique<OGLDepthPicking>();
            break;
		case COLOR_PICKING:
			return std::make_unique<OGLColorPicking>();
			break;
		case RAY_PICKING:
			return std::make_unique<OGLRayPicking>();
			break;
		case REFRACTION_REFLECTION:
			return std::make_unique<OGLRefractionReflection>();
			break; 
        default:
            return nullptr;
            break;
    }
    return nullptr;
}
