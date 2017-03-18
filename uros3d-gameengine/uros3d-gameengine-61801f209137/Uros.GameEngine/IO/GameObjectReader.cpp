#include "GameObjectReader.h"

GameEngine::GameObjectReader::GameObjectReader(const QString& path)
	: _path(path) {}

GameEngine::GameObjectReader::~GameObjectReader() {}

const QString& GameEngine::GameObjectReader::path() const
{
	return _path;
}
