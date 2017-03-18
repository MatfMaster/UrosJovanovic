#pragma once
#include "Geometry/Ray3D.h"

namespace GameEngine {
	class GameObject;
	class Raycast final
	{
		Ray3D _ray;
		GameObject* _gameObject;
		QVector3D _hitPoint;

	public:
		EXPORT Raycast(const Ray3D& ray);
		EXPORT ~Raycast();
		EXPORT bool hit() const;
		EXPORT GameObject* gameObject() const;
		EXPORT const QVector3D& hitPoint() const;

	private:
		GameObject* doRaycast();
	};
}