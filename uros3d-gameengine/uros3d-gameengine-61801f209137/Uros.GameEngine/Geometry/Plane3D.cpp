#include "Plane3D.h"

GameEngine::Plane3D::Plane3D()
	: Plane3D(QVector3D(0, 1, 0), 0) {}

GameEngine::Plane3D::Plane3D(const QVector3D& normal, float d)
{
	_a = normal.x();
	_b = normal.y();
	_c = normal.z();
	_d = d;
}

GameEngine::Plane3D::Plane3D(float a, float b, float c, float d)
	: Plane3D(QVector3D(a, b, c), d) {}

GameEngine::Plane3D::Plane3D(const QVector3D& normal, const QVector3D& point)
	: Plane3D(normal, QVector3D::dotProduct(normal, point)) {}

GameEngine::Plane3D::Plane3D(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3)
	: Plane3D(QVector3D::normal(p3 - p1, p2 - p1), p1) {}

float GameEngine::Plane3D::a() const
{
	return _a;
}

float GameEngine::Plane3D::b() const
{
	return _b;
}

float GameEngine::Plane3D::c() const
{
	return _c;
}

float GameEngine::Plane3D::d() const
{
	return _d;
}

QVector3D GameEngine::Plane3D::point() const
{
	return normal() * _d;
}

QVector3D GameEngine::Plane3D::normal() const
{
	return QVector3D(_a, _b, _c);
}

float GameEngine::Plane3D::signedDistanceToPoint(const QVector3D& point) const
{
	return QVector3D::dotProduct(normal(), point - this->point());
}

float GameEngine::Plane3D::unsignedDistanceToPoint(const QVector3D& point) const
{
	return fabs(signedDistanceToPoint(point));
}

bool GameEngine::Plane3D::isPointOnPlane(const QVector3D& point, float eps) const
{
	return relationToPoint(point, eps) == OnPlane;
}

GameEngine::Plane3D::PlaneRelationType GameEngine::Plane3D::relationToPoint(const QVector3D& point, float eps) const
{
	float dist = signedDistanceToPoint(point);
	if (fabs(dist) <= eps)
		return OnPlane;
	if (dist > 0)
		return InFrontOfPlane;
	return BehindThePlane;
}
