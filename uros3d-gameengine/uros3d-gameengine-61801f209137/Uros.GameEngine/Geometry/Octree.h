#pragma once
#include <QVector>
#include "Includes.h"
#include "OctreeNode.h"

namespace GameEngine {
	class GameObject;

	class Octree final : public OctreeNode
	{
		NOCOPY(Octree)

	public:
		Octree();
		~Octree();
		bool findGameObject(GameObject* gameObject, QVector<OctreeNode*>& nodes) const;
		bool raycast(const Ray3D& ray, GameObject*& gameObject, QVector3D* hitPoint) const;
		void intersect(const CameraFrustum& frustum, QList<GameObject*>& gameObjects) const;
		void add(GameObject* gameObject);
		void remove(GameObject* gameObject);
		void update(GameObject* gameObject);
		void initialize(const QVector<GameObject*>& gameObjects);
		void initialize(const QVector<GameObject*>& gameObjects, const QVector3D& min, const QVector3D& max);

	private:
		QHash<GameObject*, QVector<OctreeNode*>> _mapping;
		QSet<GameObject*> _outliers;
		bool _initialized;
	};
}
