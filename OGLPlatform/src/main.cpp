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
	app.SetApplicationTitle("OpenGL Window");
	if (!app.Init()) {
        std::getchar();
		return EXIT_FAILURE;
	}
    std::cout << OpenGLInfos::GetInfos() << std::endl;
	return app.Run();
}
