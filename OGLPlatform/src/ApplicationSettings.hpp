#pragma once

#include <glm/glm.hpp>
#include <string>

namespace gs
{
    class ApplicationSettings
    {
    public:
        ApplicationSettings();
        ~ApplicationSettings();
        
        bool LoadSettings(std::string path);
        
        glm::ivec2 GetStageBarPosition() const { return stageBarPosition; }
        
    private:
        glm::ivec2 stageBarPosition;
    };
}