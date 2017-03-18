#pragma once
#include "Includes.h"
#include "Component.h"

namespace GameEngine {
	class Scene;
	class Debugger final : public Component
	{
		NOCOPY(Debugger)

	public:
		EXPORT explicit Debugger(GameObject* gameObject);

	protected:
		~Debugger() override;

	public:
		EXPORT ComponentType type() const override;
		EXPORT bool isSingular() const override;
		EXPORT bool isGizmoVisible() const;
		EXPORT bool isOctreeNodeVisible() const;
		EXPORT bool isBoundingBoxVisible() const;
		EXPORT void showGizmo();
		EXPORT void showOctreeNode();
		EXPORT void showBoundingBox();
		EXPORT void hideGizmo();
		EXPORT void hideOctreeNode();
		EXPORT void hideBoundingBox();
		void render(const Scene* scene) const;

	private:
		bool _gizmo;
		bool _octree;
		bool _bbox;
	};
}
