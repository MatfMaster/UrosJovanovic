#include <QQuaternion>
#include "Segment3D.h"

GameEngine::Segment3D::Segment3D() {}

GameEngine::Segment3D::Segment3D(const QVector3D& a, const QVector3D& b)
	:_a(a),
	 _b(b) {}

const QVector3D& GameEngine::Segment3D::a() const
{
	return _a;
}

const QVector3D& GameEngine::Segment3D::b() const
{
	return _b;
}

float GameEngine::Segment3D::length() const
{
	return sqrtf(lengthSquared());
}

float GameEngine::Segment3D::lengthSquared() const
{
	return (_b - _a).lengthSquared();
}

void GameEngine::Segment3D::approximateCircle(const QVector3D& center, const QVector3D& normal, float radius, int segmentCount, QVector<Segment3D>& segments)
{
	const float PI = 3.1415926;
	const float step = 360.0f / segmentCount * (PI / 180.0f);
	float x, z;
	float r = radius;
	auto rot = QQuaternion::rotationTo(QVector3D(0, 1, 0), normal.normalized());
	QVector<QVector3D> verts;
	for (int i = 0; i <= segmentCount; i++)
	{
		x = r * cos(i * step);
		z = r * sin(i * step);
		auto vert = QVector3D(x, 0, z);
		vert = rot.rotatedVector(vert);
		vert += center;
		verts.push_back(vert);
	}
	for (int i = 0; i < verts.count() - 1; i++)
		segments.push_back(Segment3D(verts[i], verts[i + 1]));
}
