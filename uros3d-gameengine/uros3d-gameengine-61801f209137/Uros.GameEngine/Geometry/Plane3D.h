#pragma once
#include <QVector3D>
#include "Includes.h"

namespace GameEngine {
	/*
	Represents a plane in 3D space defined by equation A*x + B*y + C*z + d = 0. This class is immutable. 
	*/
	class Plane3D
	{
		float _a;
		float _b;
		float _c;
		float _d;

	public:
		enum PlaneRelationType
		{
			OnPlane = 0,
			InFrontOfPlane = 1,
			BehindThePlane = -1
		};

		EXPORT Plane3D();
		EXPORT Plane3D(const QVector3D& normal, float d);
		EXPORT Plane3D(float a, float b, float c, float d);
		EXPORT Plane3D(const QVector3D& point, const QVector3D& normal);
		EXPORT Plane3D(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3);

		EXPORT float a() const;
		EXPORT float b() const;
		EXPORT float c() const;
		EXPORT float d() const;
		EXPORT QVector3D point() const;
		EXPORT QVector3D normal() const;

		EXPORT float signedDistanceToPoint(const QVector3D& point) const;
		EXPORT float unsignedDistanceToPoint(const QVector3D& point) const;
		EXPORT bool isPointOnPlane(const QVector3D& point, float eps = 0.001) const;
		EXPORT PlaneRelationType relationToPoint(const QVector3D& point, float eps = 0.001) const;
	};
}
