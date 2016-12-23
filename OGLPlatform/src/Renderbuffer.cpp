#include "Renderbuffer.hpp"

namespace gs
{
	Renderbuffer::Renderbuffer()
	{
		glGenRenderbuffers(1, &renderbufferID);
	}

	Renderbuffer::~Renderbuffer()
	{
		glDeleteRenderbuffers(1, &renderbufferID);
	}
	void Renderbuffer::BindRenderbuffer(GLenum target)
	{
		renderbufferTarget = target;
		glBindRenderbuffer(renderbufferTarget, renderbufferID);
	}
}