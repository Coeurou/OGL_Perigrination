#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include "Uncopyable.hpp"
#include "Context.hpp"

namespace gs
{
    class Window : public Uncopyable
    {
    public:
        Window() : context(nullptr) {}
        Window(gs::Context ctxt) : context(nullptr) {}
        virtual ~Window() {}
        
        int GetWindowWidth() const { return context->GetContextParams().windowWidth; }
        int GetWindowHeight() const { return context->GetContextParams().windowHeight; }
        void SetWindowTitle(const std::string& wTitle) { title = wTitle; }
        virtual bool CreateWindow();
        
    protected:
        std::string title;
        std::shared_ptr<gs::Context> context;
    };
}