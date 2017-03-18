#pragma once
#include <QImage>
#include "Includes.h"

namespace GameEngine {
	class RenderBufferFormat final
	{
		bool _depth;
		int _samples;
	public:
		EXPORT RenderBufferFormat();
		EXPORT RenderBufferFormat(bool depth);
		EXPORT RenderBufferFormat(bool depth, int samples);
		EXPORT bool depth() const;
		EXPORT int samples() const;
	};

	class RenderBuffer
	{
		NOCOPY(RenderBuffer)

	protected:
		int _width;
		int _height;
		RenderBufferFormat _format;

		RenderBuffer(int width, int height);
		RenderBuffer(int width, int height, RenderBufferFormat format);

	public:
		EXPORT virtual ~RenderBuffer();
		EXPORT int width() const;
		EXPORT int height() const;
		EXPORT const RenderBufferFormat& format() const;
		EXPORT virtual int handle() const = 0;
		EXPORT virtual void bind() = 0;
		EXPORT virtual void unbind() = 0;
		EXPORT virtual QImage toImage() const = 0;
	};
}
