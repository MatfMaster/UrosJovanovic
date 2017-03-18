#include "Texture.h"

Texture::Texture()
	:Texture("") {}

Texture::Texture(const QString& path)
	:_path(path),
	 _tileX(1),
	 _tileY(1) {}

bool Texture::isEmpty() const
{
	return _path.isEmpty();
}

const QString& Texture::path() const
{
	return _path;
}

float Texture::getTileX() const
{
	return _tileX;
}

float Texture::getTileY() const
{
	return _tileY;
}

void Texture::setTile(float x, float y)
{
	setTileX(x);
	setTileY(y);
}

void Texture::setTileX(float tileX)
{
	_tileX = tileX;
}

void Texture::setTileY(float tileY)
{
	_tileY = tileY;
}

bool Texture::operator==(const Texture& other) const
{
	return _path == other._path && _tileX == other._tileX && _tileY == other._tileY;
}

bool Texture::operator!=(const Texture& other) const
{
	return !(*this == other);
}
