#include <QOpenGLFramebufferObject>
#include "RenderBufferGL.h"

GameEngine::RenderBufferGL::RenderBufferGL(int width, int height)
	: RenderBufferGL(width, height, RenderBufferFormat()) {}

GameEngine::RenderBufferGL::RenderBufferGL(int width, int height, RenderBufferFormat format)
	: RenderBuffer(width, height, format)
{
	_fboAccum = nullptr;
	QOpenGLFramebufferObjectFormat fboFormat;
	fboFormat.setTextureTarget(GL_TEXTURE_2D);
	if (_format.depth())
		fboFormat.setAttachment(QOpenGLFramebufferObject::Depth);
	if (_format.samples() > 0)
	{
		_fboAccum = new QOpenGLFramebufferObject(width, height, fboFormat);
		fboFormat.setSamples(_format.samples());
	}
	_fbo = new QOpenGLFramebufferObject(width, height, fboFormat);
}

GameEngine::RenderBufferGL::~RenderBufferGL()
{
	delete _fbo;
	delete _fboAccum;
}

int GameEngine::RenderBufferGL::handle() const
{
	return _fbo->handle();
}

void GameEngine::RenderBufferGL::bind()
{
	_fbo->bind();
}

void GameEngine::RenderBufferGL::unbind()
{
	_fbo->bindDefault();
}

QImage GameEngine::RenderBufferGL::toImage() const
{
	return _fbo->toImage();
}

int GameEngine::RenderBufferGL::textureID()
{
	if (_fboAccum)
	{
		QOpenGLFramebufferObject::blitFramebuffer(_fboAccum, _fbo);
		return _fboAccum->texture();
	}
	return _fbo->texture();
}
