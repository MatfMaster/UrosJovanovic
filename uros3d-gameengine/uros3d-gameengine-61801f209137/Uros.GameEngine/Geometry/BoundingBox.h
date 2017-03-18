#pragma once
#include <QVector>
#include <QVector3D>
#include "Segment3D.h"

namespace GameEngine {
	/*
	Represents an axis aligned bounding box (AABB). This class is immutable.
	*/
	class BoundingBox final
	{
		QVector3D _min;
		QVector3D _max;
		QVector3D _mid;
		QVector3D _extent;

	public:
		EXPORT BoundingBox();
		EXPORT BoundingBox(const QVector3D& min, const QVector3D& max);
		EXPORT static BoundingBox create(const float* vertices, int count);
		EXPORT static BoundingBox create(const QVector<QVector3D>& vertices);
		EXPORT static BoundingBox create(const QList<QVector3D>& vertices);
		EXPORT static bool intersect(const BoundingBox& a, const BoundingBox& b);
		EXPORT static bool isInsideOf(const BoundingBox& a, const BoundingBox& b);
		EXPORT static bool isPointInside(const BoundingBox& box, const QVector3D& point);

		EXPORT const QVector3D& minPoint() const;
		EXPORT const QVector3D& maxPoint() const;
		EXPORT const QVector3D& midPoint() const;
		EXPORT const QVector3D& extent() const;

		EXPORT void getVertices(QVector<QVector3D>& vertices) const;
		EXPORT void getSegments(QVector<Segment3D>& segments) const;
	};
}
