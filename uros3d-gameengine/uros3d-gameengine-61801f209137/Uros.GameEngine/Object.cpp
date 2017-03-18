#include "object.h"

GameEngine::Object::Object()
{
	_id = QUuid::createUuid();
}

GameEngine::Object::~Object() {}

void GameEngine::Object::destroy(Object* object)
{
	delete object;
}

const QUuid& GameEngine::Object::id() const
{
	return _id;
}
