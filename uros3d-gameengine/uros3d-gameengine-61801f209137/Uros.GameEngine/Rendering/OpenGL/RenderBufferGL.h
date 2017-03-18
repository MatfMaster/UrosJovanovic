#pragma once
#include "Rendering/RenderBuffer.h"

namespace GameEngine {
	class RenderBufferGL final : public RenderBuffer
	{
		NOCOPY(RenderBufferGL)

		QOpenGLFramebufferObject* _fbo;
		QOpenGLFramebufferObject* _fboAccum;

	public:
		RenderBufferGL(int width, int height);
		RenderBufferGL(int width, int height, RenderBufferFormat format);
		~RenderBufferGL() override;
		int handle() const override;
		void bind() override;
		void unbind() override;
		QImage toImage() const override;
		EXPORT int textureID();
	};
}
