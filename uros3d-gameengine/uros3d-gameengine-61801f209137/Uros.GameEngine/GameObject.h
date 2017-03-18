#pragma once
#include <QList>
#include "Includes.h"
#include "Component.h"
#include "Transform.h"
#include "Geometry/BoundingBox.h"

namespace GameEngine {
	class GameObject final : public QObject
	{
		Q_OBJECT
		NOCOPY(GameObject)

	public:
		enum PrimitiveType
		{
			Cube,
			Plane,
			Sphere
		};

	private:
		QString _name;
		Transform* _transform;
		QList<Component*> _components;
		BoundingBox _bbox;
		bool _bboxValid;
		bool _static;
		EXPORT Component* addComponent(Component* component);

	protected:
		~GameObject() override;

	public:
		/*
		Create a primitive game object.
		*/
		EXPORT static GameObject* createPrimitive(const PrimitiveType& type);
		/*
		Create a primitive game object with predefined position.
		*/
		EXPORT static GameObject* createPrimitive(const PrimitiveType& type, const QVector3D& position);
		/*
		Destroy a game object and all components attached to it.
		*/
		EXPORT static void destroy(GameObject* gameObject);
		/*
		Create an empty unnamed game object.
		*/
		EXPORT GameObject();
		/*
		Create an empty named game object.
		*/
		EXPORT GameObject(const QString& name);
		/*
		Get game object's name.
		*/
		EXPORT const QString& getName() const;
		/*
		Get game object's transform component. This is equivalent to calling getComponent<Transform>().
		*/
		EXPORT Transform* transform() const;
		/*
		Get all components attached to this game object.
		*/
		EXPORT const QList<Component*>& getComponents() const;
		/*
		An axis aligned bounding box.
		*/
		EXPORT const BoundingBox& boundingBox();
		/*
		Is object static? Static objects can't be moved, scaled or rotated.
		*/
		EXPORT bool isStatic() const;
		/*
		Set game object's name.
		*/
		EXPORT void setName(const QString& name);
		/*
		Mark game object as static. Static objects can't be moved, scaled or rotated.
		Marking object as static allows engine to perform additional optimizations at runtime.
		*/
		EXPORT void markAsStatic();
		/*
		Mark game object as dynamic. Dynamic objects can be moved, scaled and rotated.
		All game objects are dynamic by default.
		*/
		EXPORT void markAsDynamic();

		template <typename T>
		/*
		Get the first occurrence of component T attached to this game object. T must be derived from Component class.
		*/
		T* getComponent() const
		{
			static_assert(IS_OF_TYPE(T, Component), "Type must be derived from Component class");

			for (auto component : getComponents())
				if (auto t = dynamic_cast<T*>(component))
					return t;
			return nullptr;
		}

		template <typename T>
		/*
		Get all occurrences of component T attached to this game object. T must be derived from Component class.
		*/
		QList<T*> getComponents() const
		{
			static_assert(IS_OF_TYPE(T, Component), "Type must be derived from Component class");

			QList<T*> result;
			for (auto component : getComponents())
				if (auto t = dynamic_cast<T*>(component))
					result.push_back(t);
			return result;
		}

		template <typename T>
		/*
		Add component T to this game object. T must be derived from Component class.
		*/
		T* addComponent()
		{
			static_assert(IS_OF_TYPE(T, Component), "Type must be derived from Component class");

			return dynamic_cast<T*>(addComponent(new T(this)));
		}

		/* Internal stuff, don't call directly from API */

		bool removeComponent(Component* component);

		signals:
		void componentAdded(GameObject* gameObject, Component* component);
		void componentRemoved(GameObject* gameObject, Component* component);

	private slots:
		void onTransformChanged(Transform* transform);
	};
}
