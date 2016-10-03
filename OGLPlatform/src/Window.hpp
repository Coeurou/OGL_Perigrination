#pragma once

#include <string>
#include <memory>
#include "Uncopyable.hpp"
#include "Context.hpp"

namespace gs
{
    class Window : public Uncopyable
    {
    public:
        Window() : context(nullptr), isRunning(true) {}
        Window(gs::Context* ctxt) : Window() {}
        virtual ~Window() {}
        
        int GetWindowWidth() const { return context->GetContextParams().windowWidth; }
        int GetWindowHeight() const { return context->GetContextParams().windowHeight; }
        void SetWindowTitle(const std::string& wTitle) { title = wTitle; }
		virtual bool CreateWindow() { return true; }
		virtual bool Render() { return true; }
        
    protected:
        std::string title;
		std::shared_ptr<gs::Context> context;
		bool isRunning;
    };
}