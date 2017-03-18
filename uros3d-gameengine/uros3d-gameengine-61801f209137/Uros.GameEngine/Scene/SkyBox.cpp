#include "SkyBox.h"

GameEngine::SkyBox::SkyBox() {}

GameEngine::SkyBox::SkyBox(const QColor& color)
	:_backColor(color) {}

GameEngine::SkyBox::SkyBox(const QVector<Texture>& textures)
	:_textures(textures) {}

GameEngine::SkyBox::SkyBox(const Texture& top,
                           const Texture& bottom,
                           const Texture& front,
                           const Texture& back,
                           const Texture& left,
                           const Texture& right)
{
	_textures.clear();
	_textures.push_back(top);
	_textures.push_back(bottom);
	_textures.push_back(front);
	_textures.push_back(back);
	_textures.push_back(left);
	_textures.push_back(right);
}

const QColor& GameEngine::SkyBox::backgroundColor() const
{
	return _backColor;
}

const QVector<Texture>& GameEngine::SkyBox::textures() const
{
	return _textures;
}

const Texture& GameEngine::SkyBox::top() const
{
	Q_ASSERT(_textures.count() == 6);
	return _textures[0];
}

const Texture& GameEngine::SkyBox::bottom() const
{
	Q_ASSERT(_textures.count() == 6);
	return _textures[1];
}

const Texture& GameEngine::SkyBox::front() const
{
	Q_ASSERT(_textures.count() == 6);
	return _textures[2];
}

const Texture& GameEngine::SkyBox::back() const
{
	Q_ASSERT(_textures.count() == 6);
	return _textures[3];
}

const Texture& GameEngine::SkyBox::left() const
{
	Q_ASSERT(_textures.count() == 6);
	return _textures[4];
}

const Texture& GameEngine::SkyBox::right() const
{
	Q_ASSERT(_textures.count() == 6);
	return _textures[5];
}
