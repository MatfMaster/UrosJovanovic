#pragma once
#include <QString>
#include "Scene/Scene.h"

namespace GameEngine {
	class Project final
	{
		NOCOPY(Project)

		QString _name;
		Scene* _activeScene;

	public:
		EXPORT Project();
		EXPORT Project(const char* name);
		EXPORT ~Project();

		EXPORT const QString& getName() const;
		EXPORT void setName(const QString& name);

		EXPORT Scene* getActiveScene() const;
		EXPORT void setActiveScene(Scene* scene);
	};
}
