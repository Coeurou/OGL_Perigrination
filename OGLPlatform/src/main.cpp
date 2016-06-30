#include "OGLApplication.hpp"
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif // __APPLE__

int main(int args, char** argc, char** argv)
{
	OGLApplication app;
	app.SetApplicationTitle("OpenGL Window");
	if (!app.Init()) {
		return EXIT_FAILURE;
	}
	return app.Run();
}
