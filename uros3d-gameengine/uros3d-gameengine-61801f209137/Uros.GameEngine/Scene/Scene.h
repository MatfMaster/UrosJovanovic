#pragma once
#include <QObject>
#include "Includes.h"
#include "Geometry/Octree.h"

namespace GameEngine {
	class Component;
	class Transform;
	class Camera;
	class Light;
	class Behaviour;
	class Renderer;
	class Debugger;
	class GameObject;
	class GameObjectReader;

	class Scene final : QObject
	{
		Q_OBJECT
		NOCOPY(Scene)

	public:
		EXPORT Scene();
		EXPORT Scene(const QString& name);
		~Scene();

		/*
		Get scene's name.
		*/
		EXPORT const QString& getName() const;
		/*
		Get a list of all lights in scene.
		*/
		EXPORT const QList<Light*>& lights() const;
		/*
		Get a list of all cameras in scene.
		*/
		EXPORT const QList<Camera*>& cameras() const;
		/*
		Get a list of all game objects in scene.
		*/
		EXPORT const QList<GameObject*>& gameObjects() const;
		/*
		Returns a pointer to the first game object with specified name, NULL if no such object exists in scene.
		*/
		EXPORT GameObject* findGameObject(const QString& name) const;
		/*
		Returns all game object with specified name. If there are no such objects in scene, empty list is returned.
		*/
		EXPORT QList<GameObject*> findGameObjects(const QString& name) const;
		/*
		Set scene's name.
		*/
		EXPORT void setName(const QString& name);

		/* Internal stuff, don't call from API */

		void initialize();
		const Octree& octree() const;
		void addGameObject(GameObject* gameObject);
		void removeGameObject(GameObject* gameObject);
		void update(double deltaTime) const;
		void render();

	private slots:
		void onComponentAdded(GameObject* gameObject, Component* component);
		void onComponentRemoved(GameObject* gameObject, Component* component);
		void onTransformChanged(Transform* transform);

	private:
		QString _name;
		QList<GameObject*> _gameObjects;
		QList<Camera*> _cameras;
		QList<Light*> _lights;
		QList<Behaviour*> _behaviours;
		QList<Debugger*> _debuggers;
		QList<Renderer*> _transparentObjects;
		QList<GameObject*> _visibleObjects;
		Octree _octree;
	};
}
