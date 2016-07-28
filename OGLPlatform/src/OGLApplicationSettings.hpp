#pragma once

#include <glm/glm.hpp>
#include <string>

class OGLApplicationSettings
{
public:
	OGLApplicationSettings();
	~OGLApplicationSettings();

	bool LoadSettings(std::string path);

	glm::ivec2 GetStageBarPosition() const { return stageBarPosition; }

private:
	glm::ivec2 stageBarPosition;
};