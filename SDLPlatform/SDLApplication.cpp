#include "SDLApplication.h"
#include <stdlib.h>

SDLApplication::SDLApplication() : isRunning(true)
{
}

SDLApplication::~SDLApplication()
{
}

bool SDLApplication::Init()
{
	return false;
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
	}
	return EXIT_SUCCESS;
}

void SDLApplication::PollEvent(const SDL_Event& event)
{
}

void SDLApplication::Loop()
{
}

void SDLApplication::Render()
{
}

void SDLApplication::CleanUp()
{
}
