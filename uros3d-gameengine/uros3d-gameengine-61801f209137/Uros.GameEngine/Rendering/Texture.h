#pragma once
#include <QString>
#include "Includes.h"

class Texture final
{
	QString _path;
	float _tileX;
	float _tileY;

public:
	EXPORT Texture();
	EXPORT Texture(const QString& path);

	EXPORT bool isEmpty() const;
	EXPORT const QString& path() const;
	EXPORT float getTileX() const;
	EXPORT float getTileY() const;

	EXPORT void setTile(float tileX, float tileY);
	EXPORT void setTileX(float tileX);
	EXPORT void setTileY(float tileY);

	EXPORT bool operator ==(const Texture& other) const;
	EXPORT bool operator !=(const Texture& other) const;
};
