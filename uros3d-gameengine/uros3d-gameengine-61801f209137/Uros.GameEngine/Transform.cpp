#include <QQueue>
#include <QMatrix4x4>
#include "Transform.h"
#include "GameObject.h"

using namespace GameEngine;

QVector3D Transform::_up = QVector3D(0, 1, 0);
QVector3D Transform::_right = QVector3D(1, 0, 0);
QVector3D Transform::_forward = QVector3D(0, 0, 1);

const QVector3D& Transform::up()
{
	return _up;
}

const QVector3D& Transform::right()
{
	return _right;
}

const QVector3D& Transform::forward()
{
	return _forward;
}

Transform::Transform(GameObject* gameObject)
	: Component(gameObject), _scale(1, 1, 1), _parent(nullptr)
{
	_rebuildTransformMatrix();
}

Transform::~Transform()
{
	setParent(nullptr);
	for (auto child : _children)
		delete child;
}

QList<Transform*> Transform::_getAllChildren() const
{
	QList<Transform*> allChildren;
	QQueue<Transform*> children;
	for (auto child : _children)
		children.enqueue(child);
	while (!children.isEmpty())
	{
		Transform* transform = children.dequeue();
		for (auto child : transform->children())
			children.enqueue(child);
		allChildren.push_back(transform);
	}
	return allChildren;
}

void Transform::_addChild(Transform* transform)
{
	if (!_children.contains(transform))
		_children.push_back(transform);
}

void Transform::_removeChild(Transform* transform)
{
	if (_children.contains(transform))
		_children.removeOne(transform);
}

void Transform::_removeChildRecursively(Transform* transform)
{
	for (auto _child : children())
		if (_child == transform)
		{
			_removeChild(_child);
			return;
		}
		else
			_child->_removeChildRecursively(transform);
}

void Transform::_rebuildTransformMatrix()
{
	_matrix.setToIdentity();
	_matrix.translate(_position);
	_matrix.rotate(_rotation);
	_matrix.scale(_scale);
	emit changed(this);
}

Component::ComponentType Transform::type() const
{
	return T_TRANSFORM;
}

bool Transform::isSingular() const
{
	return true;
}

Transform* Transform::getParent() const
{
	return _parent;
}

const QList<Transform*>& Transform::children() const
{
	return _children;
}

QVector3D Transform::getLocalPosition() const
{
	return _parent ? _position - _parent->getPosition() : _position;
}

QQuaternion Transform::getLocalRotation() const
{
	return _parent ? _rotation * _parent->getRotation().conjugated() : _rotation;
}

QVector3D Transform::getLocalEulerAngles() const
{
	return getLocalRotation().toEulerAngles();
}

QVector3D Transform::getLocalScale() const
{
	if (_parent)
	{
		auto parentScale = _parent->getScale();
		float scaleX = parentScale.x() == 0 ? 0 : 1.0 / parentScale.x();
		float scaleY = parentScale.y() == 0 ? 0 : 1.0 / parentScale.y();
		float scaleZ = parentScale.z() == 0 ? 0 : 1.0 / parentScale.z();
		return QVector3D(_scale.x() * scaleX, _scale.y() * scaleY, _scale.z() * scaleZ);
	}
	return _scale;
}

const QVector3D& Transform::getPosition() const
{
	return _position;
}

const QQuaternion& Transform::getRotation() const
{
	return _rotation;
}

QVector3D Transform::getEulerAngles() const
{
	return _rotation.toEulerAngles();
}

const QVector3D& Transform::getScale() const
{
	return _scale;
}

const QMatrix4x4& Transform::getMatrix() const
{
	return _matrix;
}

QVector3D Transform::getUp() const
{
	return _rotation.rotatedVector(_up);
}

QVector3D Transform::getRight() const
{
	return _rotation.rotatedVector(_right);
}

QVector3D Transform::getForward() const
{
	return _rotation.rotatedVector(_forward);
}

void Transform::addChild(Transform* child)
{
	child->setParent(this);
}

void Transform::removeChild(Transform* child)
{
	if (_children.contains(child))
		child->setParent(nullptr);
}

void Transform::removeChildRecursively(Transform* child)
{
	for (auto _child : _getAllChildren())
		if (_child == child)
		{
			child->setParent(nullptr);
			return;
		}
}

void Transform::setParent(Transform* parent)
{
	if (gameObject()->isStatic())
	{
		ERROR_LOG("Transform::setParent() Can't change static object's parent.");
		return;
	}

	//TODO: Adjust scale, rotation and translation

	//Avoid infinite loop
	if (_parent == parent)
		return;
	//No "Televisa presenta" here
	if (parent == this)
		return;
	//Parent cannot be contained in lower levels
	if (parent)
		for (auto child : _getAllChildren())
			if (child == parent)
				return;
	//Transform can have no parent
	if (_parent)
		_parent->_removeChild(this);
	_parent = parent;
	if (_parent)
		_parent->_addChild(this);

	emit changed(this);
}

void Transform::setLocalPosition(const QVector3D& position)
{
	auto offset = position - getLocalPosition();
	setPosition(_position + offset);
}

void Transform::setLocalRotation(const QQuaternion& rotation)
{
	auto relative = getLocalRotation().inverted() * rotation;
	setRotation(_rotation * relative);
}

void Transform::setLocalEulerAngles(const QVector3D& eulerAngles)
{
	auto rotation = QQuaternion::fromEulerAngles(eulerAngles);
	setLocalRotation(rotation);
}

void Transform::setLocalScale(const QVector3D& scale)
{
	auto localScale = getLocalScale();
	float scaleX = localScale.x() == 0 ? 0 : scale.x() / localScale.x();
	float scaleY = localScale.y() == 0 ? 0 : scale.y() / localScale.y();
	float scaleZ = localScale.z() == 0 ? 0 : scale.z() / localScale.z();
	setScale(QVector3D(_scale.x() * scaleX, _scale.y() * scaleY, _scale.z() * scaleZ));
}

void Transform::setPosition(const QVector3D& position)
{
	if (gameObject()->isStatic())
	{
		ERROR_LOG("Transform::setPosition() Can't change static object's position.");
		return;
	}
	auto offset = position - _position;
	_position = position;
	for (auto child : _children)
		child->translate(offset, World);
	_rebuildTransformMatrix();
}

void Transform::setRotation(const QQuaternion& rotation)
{
	if (gameObject()->isStatic())
	{
		ERROR_LOG("Transform::setRotation() Can't change static object's rotation.");
		return;
	}
	auto relative = _rotation.inverted() * rotation;
	_rotation = rotation;
	for (auto child : _children)
	{
		QVector3D vec = child->getPosition() - getPosition();
		vec = relative * vec;
		child->setPosition(getPosition() + vec);
		child->setRotation(relative * child->getRotation());
	}
	_rebuildTransformMatrix();
}

void Transform::setEulerAngles(const QVector3D& eulerAngles)
{
	auto rotation = QQuaternion::fromEulerAngles(eulerAngles);
	setRotation(rotation);
}

void Transform::setScale(const QVector3D& scale)
{
	if (gameObject()->isStatic())
	{
		ERROR_LOG("Transform::setScale() Can't change static object's scale.");
		return;
	}
	float scaleX = _scale.x() == 0 ? 0 : scale.x() / _scale.x();
	float scaleY = _scale.y() == 0 ? 0 : scale.y() / _scale.y();
	float scaleZ = _scale.z() == 0 ? 0 : scale.z() / _scale.z();
	_scale = scale;
	for (auto child : _children)
		child->scale(scaleX, scaleY, scaleZ);
	_rebuildTransformMatrix();
}

void Transform::translate(const float& x, const float& y, const float& z, Space relativeTo)
{
	translate(QVector3D(x, y, z), relativeTo);
}

void Transform::translate(const QVector3D& amount, Space relativeTo)
{
	if (relativeTo == Self)
		setLocalPosition(getLocalPosition() + amount);
	else
		setPosition(getPosition() + amount);
}

void Transform::translate(const QVector3D& direction, const float& amount, Space relativeTo)
{
	translate(direction.normalized() * amount, relativeTo);
}

void Transform::rotate(const float& xAngle, const float& yAngle, const float& zAngle, Space relativeTo)
{
	auto quaternion = QQuaternion::fromEulerAngles(xAngle, yAngle, zAngle);
	if (relativeTo == Self)
		setLocalRotation(quaternion * getLocalRotation());
	else
		setRotation(quaternion * getRotation());
}

void Transform::rotate(const QVector3D& eulerAngles, Space relativeTo)
{
	rotate(eulerAngles.x(), eulerAngles.y(), eulerAngles.z(), relativeTo);
}

void Transform::rotate(const QVector3D& axis, const float& angle, Space relativeTo)
{
	auto quaternion = QQuaternion::fromAxisAndAngle(axis, angle);
	if (relativeTo == Self)
		setLocalRotation(quaternion * getLocalRotation());
	else
		setRotation(quaternion * getRotation());
}

void Transform::rotateAround(const QVector3D& point, const QVector3D& axis, const float& angle)
{
	auto quaternion = QQuaternion::fromAxisAndAngle(axis, angle);
	auto vec = _position - point;
	vec = quaternion * vec;
	setPosition(point + vec);
	setRotation(quaternion * getRotation());
}

void Transform::scale(const float& x, const float& y, const float& z, Space relativeTo)
{
	if (relativeTo == Self)
		setLocalScale(QVector3D(_scale.x() * x, _scale.y() * y, _scale.z() * z));
	else
		setScale(QVector3D(_scale.x() * x, _scale.y() * y, _scale.z() * z));
}

void Transform::scale(const QVector3D& amount, Space relativeTo)
{
	scale(amount.x(), amount.y(), amount.z(), relativeTo);
}

void Transform::scale(const float& amount, Space relativeTo)
{
	scale(amount, amount, amount, relativeTo);
}

void Transform::setUp(const QVector3D& up)
{
	setRotation(QQuaternion::rotationTo(_up, up));
}

void Transform::setRight(const QVector3D& right)
{
	setRotation(QQuaternion::rotationTo(_right, right));
}

void Transform::setForward(const QVector3D& forward)
{
	setRotation(QQuaternion::rotationTo(_forward, forward));
}

void Transform::lookAt(const QVector3D& target)
{
	lookAt(target, _up);
}

void Transform::lookAt(const QVector3D& target, const QVector3D& up)
{
	QVector3D direction = (target - _position).normalized();
	setRotation(QQuaternion::fromDirection(direction, up));
}
