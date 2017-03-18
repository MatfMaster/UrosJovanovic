#pragma once
#include <QString>
#include "Includes.h"

namespace GameEngine {
	class GameObject;
	class GameObjectReader
	{
	protected:
		EXPORT explicit GameObjectReader(const QString& path);
	public:
		EXPORT virtual ~GameObjectReader();
		EXPORT virtual GameObject* read() const = 0;
		EXPORT const QString& path() const;
	private:
		QString _path;
	};
}
