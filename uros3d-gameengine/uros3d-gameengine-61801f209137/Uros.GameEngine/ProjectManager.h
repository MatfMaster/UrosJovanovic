#pragma once
#include <QMutex>
#include "Project.h"

namespace GameEngine {
	class ProjectManager final
	{
		NOCOPY(ProjectManager)

		ProjectManager();
		~ProjectManager();
		Project* _activeProject;

	public:
		EXPORT static ProjectManager* instance();
		EXPORT Project* activeProject() const;
		EXPORT void loadProject(Project* project);
		EXPORT void loadProject(const QString& path);
	};
}
