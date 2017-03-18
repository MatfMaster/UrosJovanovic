#pragma once
#include <QObject>
#include "Component.h"
#include "Material.h"
#include "Geometry/GeometryBase.h"

namespace GameEngine {
	class Transform;
	class Renderer : public QObject, public Component
	{
		Q_OBJECT
		NOCOPY(Renderer)

	protected:
		explicit Renderer(GameObject* gameObject);
		virtual ~Renderer() override;

	public:
		EXPORT ComponentType type() const override;
		EXPORT bool isSingular() const override;
		EXPORT bool isEnabled() const;
		EXPORT Material& getMaterial();
		EXPORT const Material& getConstMaterial() const;
		EXPORT void enable();
		EXPORT void disable();
		EXPORT void setMaterial(const Material& material);
		EXPORT virtual const BoundingBox& boundingBox() = 0;
		virtual void render() = 0;

	protected slots:
		virtual void onTransformChanged(Transform* transform);

	private:
		bool _enabled;
		Material _material;
	};
}
