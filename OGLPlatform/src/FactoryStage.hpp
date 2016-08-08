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
	ANIMATED_TRIANGLE,
	SIMPLE_CUBE,
	MIPMAPS,
	MIX_TEXTURE,
	RIPPLE
};

class FactoryStage : public Uncopyable
{
public:
	static std::shared_ptr<gs::Stage> CreateStage(STAGES key);

private:
	FactoryStage() {}
	~FactoryStage() {}
};