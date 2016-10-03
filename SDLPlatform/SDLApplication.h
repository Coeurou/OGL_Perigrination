#pragma once
#include <SDL.h>

class SDLApplication
{
public:
	SDLApplication();
	~SDLApplication();

	bool Init();
	int Run();
	void PollEvent(const SDL_Event& event);
	void Loop();
	void Render();
	void CleanUp();

private:
	bool isRunning;
};

