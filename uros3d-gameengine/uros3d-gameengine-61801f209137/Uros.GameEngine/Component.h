#pragma once
#include "Includes.h"

namespace GameEngine {
	class GameObject;
	class Component
	{
		NOCOPY(Component)

	public:
		enum ComponentType
		{
			T_TEST = 0,
			T_TRANSFORM = 1,
			T_RENDERER = 2,
			T_CAMERA = 3,
			T_LIGHT = 4,
			T_DEBUGGER = 5,
			T_BEHAVIOUR = 6
		};

	private:
		GameObject* _gameObject;
	protected:
		EXPORT explicit Component(GameObject* gameObject);
		EXPORT virtual ~Component();
	public:
		EXPORT GameObject* gameObject() const;
		EXPORT virtual ComponentType type() const = 0;
		EXPORT virtual bool isSingular() const;
		EXPORT static void destroy(Component* component);
	};
}
