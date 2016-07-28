#include "FactoryStage.hpp"
#include "OGLPointSize.hpp"
#include "OGLSimpleTriangle.hpp"
#include "OGLAnimatedTriangle.hpp"

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
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
