#pragma once
#include <GL/glew.h>

namespace gs
{
	class Renderbuffer
	{
	public:
		Renderbuffer();
		~Renderbuffer();

		void BindRenderbuffer(GLenum target);
		GLenum GetRenderbufferTarget() { return renderbufferTarget; }

	private:
		GLuint renderbufferID;
		GLenum renderbufferTarget;
	};
}