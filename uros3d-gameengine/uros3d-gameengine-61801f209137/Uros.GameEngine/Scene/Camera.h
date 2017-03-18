#pragma once
#include "Component.h"
#include "SkyBox.h"
#include "Geometry/Segment3D.h"
#include "Geometry/Plane3D.h"
#include "Geometry/BoundingBox.h"

namespace GameEngine {
	class Camera;

	class CameraFrustum final
	{
		NOCOPY(CameraFrustum)
		Camera* _camera;
	public:
		explicit CameraFrustum(Camera* camera);
		EXPORT Camera* camera() const;
		EXPORT BoundingBox boundingBox() const;
		EXPORT void getCorners(QVector<QVector3D>& corners) const;
		EXPORT void getSegments(QVector<Segment3D>& segments) const;
		EXPORT void getPlanes(QVector<Plane3D>& planes) const;
	};

	class Camera final : public Component
	{
		NOCOPY(Camera)

		bool _enabled;
		bool _ortho;
		float _fov;
		float _nearZ;
		float _farZ;
		float _size;
		float _ratio;
		float _zLayer;
		SkyBox _skyBox;
		CameraFrustum _frustum;

	protected:
		~Camera() override;

	public:
		explicit Camera(GameObject* gameObject);

		EXPORT static GameObject* create(bool orthographic = false);

		EXPORT ComponentType type() const override;
		EXPORT bool isSingular() const override;

		EXPORT bool isEnabled() const;
		EXPORT bool isOrthographic() const;
		EXPORT float getFOV() const;
		EXPORT float getClippingNear() const;
		EXPORT float getClippingFar() const;
		EXPORT float getSize() const;
		EXPORT float getAspectRatio() const;
		EXPORT float getZLayer() const;
		EXPORT QMatrix4x4 projectionMatrix() const;
		EXPORT const SkyBox& getSkyBox() const;
		EXPORT const CameraFrustum& frustum() const;

		EXPORT void enable();
		EXPORT void disable();
		EXPORT void enableOrthographicMode();
		EXPORT void disableOrthographicMode();
		EXPORT void setFOV(float fov);
		EXPORT void setClippingNear(float nearZ);
		EXPORT void setClippingFar(float farZ);
		EXPORT void setSize(float size);
		EXPORT void setAspectRatio(float ratio);
		EXPORT void setZLayer(float zLayer);
		EXPORT void setSkyBox(const SkyBox& skyBox);

		EXPORT QVector3D screenToWorld(const QVector3D& screen) const;
		EXPORT QVector3D worldToScreen(const QVector3D& world) const;
	};
}
