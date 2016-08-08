#include "Application.hpp"
#include "OpenGLInfos.hpp"
#include <stdlib.h>
#include <iostream>

#ifdef __APPLE__
//#include <OpenGL/OpenGL.h>
#endif // __APPLE__


int main(int args, char** argc, char** argv)
{
    gs::Application app;
	if (!app.Init()) {
        std::getchar();
		return EXIT_FAILURE;
	}
	app.SetApplicationTitle("OpenGL Window");
	std::cout << OpenGLInfos::GetInfos() << std::endl;
	return app.Run();
}
