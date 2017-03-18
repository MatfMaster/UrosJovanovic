#include "GameObject.h"
#include "Application.h"
#include "ProjectManager.h"
#include "Geometry/Mesh.h"
#include "Rendering/MeshRenderer.h"

GameEngine::GameObject::GameObject()
{
	addComponent(_transform = new Transform(this));
	if (auto project = ProjectManager::instance()->activeProject())
		if (auto scene = project->getActiveScene())
			scene->addGameObject(this);
	_bboxValid = false;
	_static = false;
}

GameEngine::GameObject::GameObject(const QString& name)
	: GameObject()
{
	setName(name);
}

GameEngine::GameObject::~GameObject()
{
	auto components = _components;
	_components.clear();
	for (auto component : components)
		Component::destroy(component);
	if (auto project = ProjectManager::instance()->activeProject())
		if (auto scene = project->getActiveScene())
			scene->removeGameObject(this);
}

const GameEngine::BoundingBox& GameEngine::GameObject::boundingBox()
{
	if (!_bboxValid)
	{
		BoundingBox bbox(_transform->getPosition(), _transform->getPosition());

		if (auto renderer = getComponent<Renderer>())
			bbox = renderer->boundingBox();

		const auto& children = _transform->children();
		QVector<QVector3D> vertices;
		for (const auto& child : children)
		{
			vertices.push_back(child->gameObject()->boundingBox().minPoint());
			vertices.push_back(child->gameObject()->boundingBox().maxPoint());
		}
		vertices.push_back(bbox.minPoint());
		vertices.push_back(bbox.maxPoint());
		_bbox = BoundingBox::create(vertices);
		_bboxValid = true;
	}
	return _bbox;
}

bool GameEngine::GameObject::isStatic() const
{
	return _static;
}

GameEngine::GameObject* GameEngine::GameObject::createPrimitive(const PrimitiveType& type)
{
	Mesh* mesh;
	QString name;
	Material material;
	switch (type)
	{
		case Cube:
			mesh = Mesh::cube();
			name = "Cube";
			break;
		case Plane:
			mesh = Mesh::plane();
			name = "Plane";
			material.enableTwoSided();
			break;
		case Sphere:
			mesh = Mesh::sphere();
			name = "Sphere";
			break;
		default:
			throw std::logic_error("GameObject::createPrimitive: Invalid primitive type.");
	}

	auto gameObject = new GameObject(name);
	auto renderer = gameObject->addComponent<MeshRenderer>();
	renderer->setMesh(mesh);
	renderer->setMaterial(material);
	return gameObject;
}

GameEngine::GameObject* GameEngine::GameObject::createPrimitive(const PrimitiveType& type, const QVector3D& position)
{
	auto gameObject = createPrimitive(type);
	gameObject->getComponent<Transform>()->setPosition(position);
	return gameObject;
}

void GameEngine::GameObject::destroy(GameObject* gameObject)
{
	delete gameObject;
}

const QString& GameEngine::GameObject::getName() const
{
	return _name;
}

void GameEngine::GameObject::setName(const QString& name)
{
	_name = name;
}

void GameEngine::GameObject::markAsStatic()
{
	if (!Application::isRunning())
		_static = true;
	else
	ERROR_LOG("GameObject::markAsStatic: This property can't be changed at runtime.");
}

void GameEngine::GameObject::markAsDynamic()
{
	if (!Application::isRunning())
		_static = false;
	else
	ERROR_LOG("GameObject::markAsDynamic: This property can't be changed at runtime.");
}

GameEngine::Component* GameEngine::GameObject::addComponent(Component* component)
{
	if (_static && Application::isRunning())
		throw std::logic_error("GameObject::addComponent: Can't add component to static object at runtime.");

	for (auto _component : _components)
		if (_component == component ||
			_component->type() == component->type() && component->isSingular())
			throw std::logic_error("GameObject::addComponent: Component is already attached.");

	if (auto transform = dynamic_cast<Transform*>(component))
		connect(transform, SIGNAL(changed(Transform*)),
		        this, SLOT(onTransformChanged(Transform*)));

	_components.push_back(component);
	emit componentAdded(this, component);
	return component;
}

const QList<GameEngine::Component*>& GameEngine::GameObject::getComponents() const
{
	return _components;
}

GameEngine::Transform* GameEngine::GameObject::transform() const
{
	return _transform;
}

bool GameEngine::GameObject::removeComponent(Component* component)
{
	if (_static && Application::isRunning())
	{
		ERROR_LOG("GameObject::removeComponent: Can't remove component from static object at runtime.");
		return false;
	}

	if (_components.removeAll(component) > 0)
	{
		emit componentRemoved(this, component);
		return true;
	}
	return false;
}

void GameEngine::GameObject::onTransformChanged(Transform* transform)
{
	// Invalidate bounding box
	_bboxValid = false;
}
