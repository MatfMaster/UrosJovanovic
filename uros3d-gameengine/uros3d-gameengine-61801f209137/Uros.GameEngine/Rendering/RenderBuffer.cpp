#include "RenderBuffer.h"

GameEngine::RenderBuffer::RenderBuffer(int width, int height)
	: RenderBuffer(width, height, RenderBufferFormat()) {}

GameEngine::RenderBuffer::RenderBuffer(int width, int height, RenderBufferFormat format)
{
	_width = width;
	_height = height;
	_format = format;
}

GameEngine::RenderBuffer::~RenderBuffer() {}

int GameEngine::RenderBuffer::width() const
{
	return _width;
}

int GameEngine::RenderBuffer::height() const
{
	return _height;
}

const GameEngine::RenderBufferFormat& GameEngine::RenderBuffer::format() const
{
	return _format;
}

GameEngine::RenderBufferFormat::RenderBufferFormat()
	: RenderBufferFormat(true, 0) {}

GameEngine::RenderBufferFormat::RenderBufferFormat(bool depth)
	: RenderBufferFormat(depth, 0) {}

GameEngine::RenderBufferFormat::RenderBufferFormat(bool depth, int samples)
	: _depth(depth),
	  _samples(samples < 0 ? 0 : samples) {}

bool GameEngine::RenderBufferFormat::depth() const
{
	return _depth;
}

int GameEngine::RenderBufferFormat::samples() const
{
	return _samples;
}
