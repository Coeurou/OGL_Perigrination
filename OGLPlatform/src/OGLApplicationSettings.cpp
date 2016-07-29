#include "OGLApplicationSettings.hpp"
#include "tinyxml2.h"
#include <iostream>

OGLApplicationSettings::OGLApplicationSettings()
{
}

OGLApplicationSettings::~OGLApplicationSettings()
{
}

bool OGLApplicationSettings::LoadSettings(std::string path)
{
	tinyxml2::XMLDocument doc;
	auto loadOK = doc.LoadFile(path.c_str());

	if (loadOK == tinyxml2::XMLError::XML_SUCCESS) {
		auto rootNode = doc.FirstChildElement("OGLPlatform");
		auto barPos = rootNode->FirstChildElement("StageBar");
		std::string xPos = barPos->FirstChildElement("X")->GetText();
		std::string yPos = barPos->FirstChildElement("Y")->GetText();

		stageBarPosition = glm::vec2(std::stoi(xPos), std::stoi(yPos));
		return true;
	}
	else {
		std::cout << "Settings loading failed error code: " << doc.ErrorName() << std::endl;
	}
	return false;
}

