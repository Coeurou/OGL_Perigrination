#pragma once
#include "OGLStage.hpp"
#include <map>
#include <memory>

enum STAGES
{
	ALL,
	POINT_SIZE,
	SIMPLE_TRIANGLE,
	ANIMATED_TRIANGLE
};

class FactoryStage : public Uncopyable
{
public:
	static std::shared_ptr<OGLStage> CreateStage(STAGES key);

private:
	FactoryStage() {}
	~FactoryStage() {}
};