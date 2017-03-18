#pragma once
#include <QList>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QObject>
#include "Component.h"

namespace GameEngine {
	class Transform final : public QObject, public Component
	{
		Q_OBJECT
		NOCOPY(Transform)

	public:
		enum Space
		{
			World,
			Self
		};

	private:
		static QVector3D _up;
		static QVector3D _right;
		static QVector3D _forward;

		QVector3D _position;
		QQuaternion _rotation;
		QVector3D _scale;
		QMatrix4x4 _matrix;
		Transform* _parent;
		QList<Transform*> _children;

		QList<Transform*> _getAllChildren() const;
		void _addChild(Transform* child);
		void _removeChild(Transform* child);
		void _removeChildRecursively(Transform* child);
		void _rebuildTransformMatrix();

	protected:
		~Transform() override;

	public:
		explicit Transform(GameObject* gameObject); //This component can only be created internaly

		EXPORT static const QVector3D& up();
		EXPORT static const QVector3D& right();
		EXPORT static const QVector3D& forward();

		EXPORT ComponentType type() const override;
		EXPORT bool isSingular() const override;
		/*
		Get parent, NULL if root.
		*/
		EXPORT Transform* getParent() const;
		/*
		Get immediate children.
		*/
		EXPORT const QList<Transform*>& children() const;
		/*
		Get position in X,Y,Z coordinates relative to local space.
		*/
		EXPORT QVector3D getLocalPosition() const;
		/*
		Get rotation as Quaternion relative to local space.
		*/
		EXPORT QQuaternion getLocalRotation() const;
		/*
		Get rotation as euler angles in degrees relative to local space.
		*/
		EXPORT QVector3D getLocalEulerAngles() const;
		/*
		Get scale relative to local space.
		*/
		EXPORT QVector3D getLocalScale() const;
		/*
		Get position in X,Y,Z coordinates relative to world space.
		*/
		EXPORT const QVector3D& getPosition() const;
		/*
		Get rotation as Quaternion relative to world space.
		*/
		EXPORT const QQuaternion& getRotation() const;
		/*
		Get rotation as euler angles in degrees relative to world space.
		*/
		EXPORT QVector3D getEulerAngles() const;
		/*
		Get scale relative to world space.
		*/
		EXPORT const QVector3D& getScale() const;
		/*
		Get transform matrix relative to world space.
		*/
		EXPORT const QMatrix4x4& getMatrix() const;
		/*
		Get transform up vector (green axis) relative to world space.
		*/
		EXPORT QVector3D getUp() const;
		/*
		Get transform right vector (red axis) relative to world space.
		*/
		EXPORT QVector3D getRight() const;
		/*
		Get transform forward vector (blue axis) relative to world space.
		*/
		EXPORT QVector3D getForward() const;

		EXPORT void addChild(Transform* child);
		EXPORT void removeChild(Transform* child);
		EXPORT void removeChildRecursively(Transform* child);
		EXPORT void setParent(Transform* parent);
		EXPORT void setLocalPosition(const QVector3D& position);
		EXPORT void setLocalRotation(const QQuaternion& rotation);
		EXPORT void setLocalEulerAngles(const QVector3D& eulerAngles);
		EXPORT void setLocalScale(const QVector3D& scale);
		EXPORT void setPosition(const QVector3D& position);
		EXPORT void setRotation(const QQuaternion& rotation);
		EXPORT void setEulerAngles(const QVector3D& eulerAngles);
		EXPORT void setScale(const QVector3D& scale);
		EXPORT void translate(const float& x, const float& y, const float& z, Space relativeTo = World);
		EXPORT void translate(const QVector3D& amount, Space relativeTo = World);
		EXPORT void translate(const QVector3D& direction, const float& amount, Space relativeTo = World);
		EXPORT void rotate(const float& xAngle, const float& yAngle, const float& zAngle, Space relativeTo = World);
		EXPORT void rotate(const QVector3D& eulerAngles, Space relativeTo = World);
		EXPORT void rotate(const QVector3D& axis, const float& angle, Space relativeTo = World);
		EXPORT void rotateAround(const QVector3D& point, const QVector3D& axis, const float& angle);
		EXPORT void scale(const float& x, const float& y, const float& z, Space relativeTo = World);
		EXPORT void scale(const QVector3D& amount, Space relativeTo = World);
		EXPORT void scale(const float& amount, Space relativeTo = World);
		EXPORT void setUp(const QVector3D& up);
		EXPORT void setRight(const QVector3D& right);
		EXPORT void setForward(const QVector3D& forward);
		EXPORT void lookAt(const QVector3D& target);
		EXPORT void lookAt(const QVector3D& target, const QVector3D& up);

		signals:
		void changed(Transform* transform);
	};
}
