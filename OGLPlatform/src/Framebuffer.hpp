#pragma once
#include <GL/glew.h>

namespace gs
{
	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();

		void BindFramebuffer(GLenum target);
		bool CheckCompleteStatus();

		GLenum GetFramebufferTarget() { return framebufferTarget; }

	private:
		GLuint framebufferID;
		GLenum framebufferTarget;
	};
}