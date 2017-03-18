#include "BoundingBox.h"

GameEngine::BoundingBox::BoundingBox() {}

GameEngine::BoundingBox::BoundingBox(const QVector3D& min, const QVector3D& max)
{
	_min = min;
	_max = max;
	_mid = (_min + _max) / 2.0f;
	_extent = _max - _min;
}

GameEngine::BoundingBox GameEngine::BoundingBox::create(const float* vertices, int count)
{
	float x, y, z;
	float xMin, yMin, zMin;
	float xMax, yMax, zMax;
	xMin = yMin = zMin = std::numeric_limits<float>::max();
	xMax = yMax = zMax = std::numeric_limits<float>::lowest();
	for (auto i = 0; i < count; i += 3)
	{
		x = vertices[i];
		y = vertices[i + 1];
		z = vertices[i + 2];
		xMin = x < xMin ? x : xMin;
		yMin = y < yMin ? y : yMin;
		zMin = z < zMin ? z : zMin;
		xMax = x > xMax ? x : xMax;
		yMax = y > yMax ? y : yMax;
		zMax = z > zMax ? z : zMax;
	}
	return BoundingBox(QVector3D(xMin, yMin, zMin), QVector3D(xMax, yMax, zMax));
}

GameEngine::BoundingBox GameEngine::BoundingBox::create(const QVector<QVector3D>& vertices)
{
	float x, y, z;
	float xMin, yMin, zMin;
	float xMax, yMax, zMax;
	xMin = yMin = zMin = std::numeric_limits<float>::max();
	xMax = yMax = zMax = std::numeric_limits<float>::lowest();
	for (auto i = 0; i < vertices.count(); i ++)
	{
		x = vertices[i][0];
		y = vertices[i][1];
		z = vertices[i][2];
		xMin = x < xMin ? x : xMin;
		yMin = y < yMin ? y : yMin;
		zMin = z < zMin ? z : zMin;
		xMax = x > xMax ? x : xMax;
		yMax = y > yMax ? y : yMax;
		zMax = z > zMax ? z : zMax;
	}
	return BoundingBox(QVector3D(xMin, yMin, zMin), QVector3D(xMax, yMax, zMax));
}

GameEngine::BoundingBox GameEngine::BoundingBox::create(const QList<QVector3D>& vertices)
{
	float x, y, z;
	float xMin, yMin, zMin;
	float xMax, yMax, zMax;
	xMin = yMin = zMin = std::numeric_limits<float>::max();
	xMax = yMax = zMax = std::numeric_limits<float>::lowest();
	for (auto vertex : vertices)
	{
		x = vertex[0];
		y = vertex[1];
		z = vertex[2];
		xMin = x < xMin ? x : xMin;
		yMin = y < yMin ? y : yMin;
		zMin = z < zMin ? z : zMin;
		xMax = x > xMax ? x : xMax;
		yMax = y > yMax ? y : yMax;
		zMax = z > zMax ? z : zMax;
	}
	return BoundingBox(QVector3D(xMin, yMin, zMin), QVector3D(xMax, yMax, zMax));
}

bool GameEngine::BoundingBox::intersect(const BoundingBox& a, const BoundingBox& b)
{
	auto d = a.midPoint() - b.midPoint();
	return fabs(d.x()) <= a.extent().x() / 2 + b.extent().x() / 2
		&& fabs(d.y()) <= a.extent().y() / 2 + b.extent().y() / 2
		&& fabs(d.z()) <= a.extent().z() / 2 + b.extent().z() / 2;
}

bool GameEngine::BoundingBox::isInsideOf(const BoundingBox& a, const BoundingBox& b)
{
	return a.minPoint().x() >= b.minPoint().x() && a.minPoint().y() >= b.minPoint().y() && a.minPoint().z() >= b.minPoint().z()
		&& a.maxPoint().x() <= b.maxPoint().x() && a.maxPoint().y() <= b.maxPoint().y() && a.maxPoint().z() <= b.maxPoint().z();
}

bool GameEngine::BoundingBox::isPointInside(const BoundingBox& box, const QVector3D& point)
{
	return point.x() >= box.minPoint().x() && point.y() >= box.minPoint().y() && point.z() >= box.minPoint().z()
		&& point.x() <= box.maxPoint().x() && point.y() <= box.maxPoint().y() && point.z() <= box.maxPoint().z();
}

const QVector3D& GameEngine::BoundingBox::minPoint() const
{
	return _min;
}

const QVector3D& GameEngine::BoundingBox::maxPoint() const
{
	return _max;
}

const QVector3D& GameEngine::BoundingBox::midPoint() const
{
	return _mid;
}

const QVector3D& GameEngine::BoundingBox::extent() const
{
	return _extent;
}

void GameEngine::BoundingBox::getVertices(QVector<QVector3D>& vertices) const
{
	/*
	*  Vertices are computed as follows:
	*     7+------+6
	*     /|     /|      y
	*    / |    / |      |
	*   / 3+---/--+2     |
	* 4+------+5 /       *---x
	*  | /    | /       /
	*  |/     |/       z
	* 0+------+1
	*
	*/
	vertices[0] = QVector3D(_min.x(), _min.y(), _max.z());
	vertices[1] = QVector3D(_max.x(), _min.y(), _max.z());
	vertices[2] = QVector3D(_max.x(), _min.y(), _min.z());
	vertices[3] = QVector3D(_min.x(), _min.y(), _min.z());
	vertices[4] = QVector3D(_min.x(), _max.y(), _max.z());
	vertices[5] = QVector3D(_max.x(), _max.y(), _max.z());
	vertices[6] = QVector3D(_max.x(), _max.y(), _min.z());
	vertices[7] = QVector3D(_min.x(), _max.y(), _min.z());
}

void GameEngine::BoundingBox::getSegments(QVector<Segment3D>& segments) const
{
	QVector<QVector3D> vertices(8);
	getVertices(vertices);

	segments[0] = Segment3D(vertices[0], vertices[1]);
	segments[1] = Segment3D(vertices[1], vertices[2]);
	segments[2] = Segment3D(vertices[2], vertices[3]);
	segments[3] = Segment3D(vertices[3], vertices[0]);

	segments[4] = Segment3D(vertices[4], vertices[5]);
	segments[5] = Segment3D(vertices[5], vertices[6]);
	segments[6] = Segment3D(vertices[6], vertices[7]);
	segments[7] = Segment3D(vertices[7], vertices[4]);

	segments[8] = Segment3D(vertices[4], vertices[0]);
	segments[9] = Segment3D(vertices[5], vertices[1]);
	segments[10] = Segment3D(vertices[6], vertices[2]);
	segments[11] = Segment3D(vertices[7], vertices[3]);
}
