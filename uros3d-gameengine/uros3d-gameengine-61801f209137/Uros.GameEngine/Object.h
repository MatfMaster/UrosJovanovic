#pragma once
#include <QtCore/QUuid>
#include <includes.h>

namespace GameEngine {
	class Object
	{
		QUuid _id;
	protected:
		Object();
		virtual ~Object();
	public:
		EXPORT static void destroy(Object* object);
		const QUuid& id() const;
	};
}
