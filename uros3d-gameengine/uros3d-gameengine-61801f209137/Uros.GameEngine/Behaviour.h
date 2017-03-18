#pragma once
#include "Component.h"

namespace GameEngine {
	class Behaviour : public Component
	{
		NOCOPY(Behaviour)

	private:
		bool _enabled;

	protected:
		EXPORT explicit Behaviour(GameObject* gameObject);
		EXPORT virtual ~Behaviour() override;

	public:
		EXPORT ComponentType type() const override;
		EXPORT virtual void startUp();
		EXPORT virtual void update(double deltaTime);
		EXPORT virtual void shutDown();
		EXPORT bool isEnabled() const;
		EXPORT void enable();
		EXPORT void disable();
	};
}
