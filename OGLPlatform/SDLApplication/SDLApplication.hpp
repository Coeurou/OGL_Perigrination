#pragma once
#include <SDL.h>

class SDLApplication
{
public:
	SDLApplication();
	~SDLApplication();

	int Run();
	bool Init();
	void PollEvent(const SDL_Event& event);
	void Loop();
	void Render();
	void Cleanup();

private:
	bool isRunning;
};