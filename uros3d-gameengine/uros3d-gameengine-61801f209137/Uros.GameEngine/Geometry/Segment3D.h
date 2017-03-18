#pragma once
#include <QVector3D>
#include "Includes.h"

namespace GameEngine {
	/*
	Represents a segment in 3D space. This class is immutable.
	*/
	class Segment3D
	{
		QVector3D _a;
		QVector3D _b;

	public:
		EXPORT Segment3D();
		EXPORT Segment3D(const QVector3D& a, const QVector3D& b);
		EXPORT const QVector3D& a() const;
		EXPORT const QVector3D& b() const;
		EXPORT float length() const;
		EXPORT float lengthSquared() const;
		EXPORT static void approximateCircle(const QVector3D& center,
		                                     const QVector3D& normal,
		                                     float radius,
		                                     int segmentCount,
		                                     QVector<Segment3D>& segments);
	};
}
