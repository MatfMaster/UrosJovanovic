#pragma once
#include "GameObjectReader.h"

namespace GameEngine {
	class GameObjectReaderOBJ : public GameObjectReader
	{
	public:
		EXPORT explicit GameObjectReaderOBJ(const QString& path);
		EXPORT ~GameObjectReaderOBJ() override = default;
		EXPORT GameObject* read() const override;
	private:
		float readFloat(const QByteArray& data, int& idx) const;
		void readFace(const QByteArray& data, int& idx, int& v, int& n) const;
	};
}
