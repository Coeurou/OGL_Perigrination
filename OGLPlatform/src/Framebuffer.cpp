#include "Framebuffer.hpp"
#include <iostream>

namespace gs
{
	Framebuffer::Framebuffer()
	{
		glGenFramebuffers(1, &framebufferID);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &framebufferID);
	}

	void Framebuffer::BindFramebuffer(GLenum target)
	{
		framebufferTarget = target;
		glBindFramebuffer(framebufferTarget, framebufferID);
	}

	bool Framebuffer::CheckCompleteStatus()
	{
		bool res = (glCheckFramebufferStatus(framebufferTarget) == GL_FRAMEBUFFER_COMPLETE);

		if (!res) {
			std::cerr << "[Framebuffer::CheckCompleteStatus] Framebuffer " << framebufferID <<
						 ", target: " << framebufferTarget << " incomplete status." << std::endl;
		}
		return res;
	}
}