#include "SDLApplication.hpp"
#include <stdlib.h>

SDLApplication::SDLApplication() : isRunning(true)
{
}

SDLApplication::~SDLApplication()
{
}

int SDLApplication::Run()
{
	if (!Init()) {
		return EXIT_FAILURE;
	}

	SDL_Event event;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			PollEvent(event);
		}
		Loop();
		Render();
	}

	Cleanup();

	return EXIT_SUCCESS;
}

bool SDLApplication::Init()
{
	return true;
}

void SDLApplication::PollEvent(const SDL_Event& event)
{}

void SDLApplication::Loop()
{}

void SDLApplication::Render()
{}

void SDLApplication::Cleanup()
{}

int main(int argc, char* argv[])
{
	SDLApplication theApp;

	return theApp.Run();
}
