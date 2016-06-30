#include "OGLApplication.hpp"
#include <stdlib.h>
#include <OpenGL/OpenGL.h>
int main(int args, char** argc, char** argv)
{
	OGLApplication app;
    app.SetApplicationTitle("OpenGL Window");
	if (!app.Init()) {
		return EXIT_FAILURE;
	}
	return app.Run();
}
