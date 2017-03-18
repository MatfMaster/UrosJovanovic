#pragma once
#include <QVector3D>
#include "Includes.h"

namespace GameEngine {
	/* Represents a ray in 3D space. This class is immutable. */
	class Ray3D
	{
	private:
		QVector3D _origin;
		QVector3D _direction;

	public:
		EXPORT Ray3D();
		EXPORT Ray3D(const QVector3D& origin, const QVector3D& direction);
		EXPORT const QVector3D& origin() const;
		EXPORT const QVector3D& direction() const;
	};
}