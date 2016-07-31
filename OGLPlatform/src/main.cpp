#include "OGLApplication.hpp"
#include <stdlib.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif // __APPLE__

int main(int args, char** argc, char** argv)
{
	OGLApplication app;
	app.SetApplicationTitle("OpenGL Window");
	if (!app.Init()) {
        std::getchar();
		return EXIT_FAILURE;
	}
	return app.Run();
}
