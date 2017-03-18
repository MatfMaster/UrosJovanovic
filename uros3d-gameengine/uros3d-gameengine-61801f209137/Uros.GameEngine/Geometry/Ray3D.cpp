#include "Ray3D.h"

GameEngine::Ray3D::Ray3D()
	: Ray3D(QVector3D(), QVector3D(0, 0, 1)) {}

GameEngine::Ray3D::Ray3D(const QVector3D& origin, const QVector3D& direction)
	: _origin(origin),
	  _direction(direction) {}

const QVector3D& GameEngine::Ray3D::origin() const
{
	return _origin;
}

const QVector3D& GameEngine::Ray3D::direction() const
{
	return _direction;
}
