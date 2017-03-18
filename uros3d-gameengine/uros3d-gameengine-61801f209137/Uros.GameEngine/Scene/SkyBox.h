#pragma once
#include <QColor>
#include <QVector>
#include "Rendering/Texture.h"

namespace GameEngine {
	class SkyBox final
	{
	public:
		EXPORT SkyBox();
		EXPORT SkyBox(const QColor& color);
		EXPORT SkyBox(const QVector<Texture>& textures);
		EXPORT SkyBox(const Texture& top,
		              const Texture& bottom,
		              const Texture& front,
		              const Texture& back,
		              const Texture& left,
		              const Texture& right);
		EXPORT const QColor& backgroundColor() const;
		EXPORT const QVector<Texture>& textures() const;
		EXPORT const Texture& top() const;
		EXPORT const Texture& bottom() const;
		EXPORT const Texture& front() const;
		EXPORT const Texture& back() const;
		EXPORT const Texture& left() const;
		EXPORT const Texture& right() const;

	private:
		QColor _backColor;
		QVector<Texture> _textures;
	};
}
