#pragma once

#include <map>
#include <memory>
#include "Uncopyable.hpp"

namespace gs {
    class Stage;
}

enum STAGES
{
	ALL,
	POINT_SIZE,
	SIMPLE_TRIANGLE,
	ANIMATED_PYRAMIDS,
	MIPMAPS,
	MIX_TEXTURE,
	RIPPLE,
    GEOMETRYSHADER,
    MODELMAT,
    INDEXEDDRAW,
    CAMERA,
	SIMPLE_LIGHT,
	SKYBOX,
	MATERIAL,
	POINTLIGHT,
	SPOTLIGHT,
    MULTIPLE_LIGHTS,
    FRUSTUM_CULLING,
	DEPTH_PICKING,
	REFRACTION_REFLECTION,
	FONT
};

class FactoryStage : public Uncopyable
{
public:
	static std::unique_ptr<gs::Stage> CreateStage(STAGES key);

private:
	FactoryStage() {}
	~FactoryStage() {}
};
