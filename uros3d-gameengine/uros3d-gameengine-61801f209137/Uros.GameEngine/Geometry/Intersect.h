#pragma once
#include <QVector3D>
#include "Includes.h"

namespace GameEngine {
	class Ray3D;
	class BoundingBox;
	class CameraFrustum;
	class Intersect final
	{
		NOCOPY(Intersect)
		Intersect();
		~Intersect();

	public:
		static bool aabbAndAABB(const BoundingBox& box1, const BoundingBox& box2);
		static bool triangleAndAABB(const QVector3D& a, const QVector3D& b, const QVector3D& c, const BoundingBox& box);
		static bool frustumAndAABB(const CameraFrustum& frustum, const BoundingBox& box);
		static bool rayAndAABB(const Ray3D& ray, const BoundingBox& box, float* t);
		static bool rayAndTriangle(const Ray3D& ray, const QVector3D& a, const QVector3D& b, const QVector3D& c, float* t);

	private:
		static void project(const QVector<QVector3D>& points, const QVector3D& axis, float* min, float* max);
	};
}
