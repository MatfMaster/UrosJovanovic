// QT
#include <QCoreApplication>
#include <QMainWindow>
#include <QApplication>

// GameEngine
#include "Application.h"
#include "ProjectManager.h"
#include "GameObject.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Rendering/MeshRenderer.h"
#include "Debugger.h"

// Demo
#include "CameraController.h"
#include "LightController.h"
#include "RotateAround.h"

using namespace GameEngine;

void printTransformTree(Transform* transform)
{
	QVector<Transform*> firstStack;
	firstStack.push_back(transform);

	QVector<QVector<Transform*>> childListStack;
	childListStack.push_back(firstStack);

	while (childListStack.count() > 0)
	{
		QVector<Transform*>& childStack = childListStack.last();

		if (childStack.count() == 0)
			childListStack.removeLast();
		else
		{
			transform = childStack[0];
			childStack.removeFirst();

			QString indent = "";
			for (int i = 0; i < childListStack.count() - 1; i++)
				indent += childListStack[i].count() > 0 ? "|  " : "   ";

			LOG(indent.toStdString() << "+- " << transform->gameObject()->getName().toStdString());

			if (transform->children().count() > 0)
				childListStack.push_back(QVector<Transform*>(transform->children().toVector()));
		}
	}
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	DEBUG_LOG(sizeof(GameObject) + sizeof(Transform));

	auto project = new Project("Demo");
	project->setActiveScene(new Scene("MainScene"));
	ProjectManager::instance()->loadProject(project);

	//Load objects into scene

	auto plane = GameObject::createPrimitive(GameObject::Plane);
	//plane->transform()->setForward(Transform::up());
	plane->transform()->scale(100);
	//plane->addComponent<Debugger>();

	auto sphere = GameObject::createPrimitive(GameObject::Sphere, Transform::up());
	sphere->addComponent<RotateAround>();
	Material& mat = sphere->getComponent<MeshRenderer>()->getMaterial();
	mat.setSpecularColor(Qt::white);
	mat.setShininess(100);
	mat.setShaderType(Material::Standard);

	auto cube = GameObject::createPrimitive(GameObject::Cube, Transform::up() + QVector3D(2, 0, 0));
	cube->transform()->setParent(sphere->transform());
	cube->addComponent<Debugger>();

	// Add camera
	auto camera = Camera::create();
	camera->transform()->setPosition(QVector3D(0, 0, 10));
	camera->addComponent<CameraController>();

	// Another cam
	camera = Camera::create(true);
	camera->transform()->setPosition(QVector3D(-5, 5, -5));
	camera->transform()->lookAt(QVector3D(), Transform::up());

	// Add lights

	Light::setAmbientLightColor(QColor(10, 10, 10));

	auto light = Light::create(Light::Spot);
	light->getComponent<Light>()->setSpotOuterAngle(30);
	light->getComponent<Light>()->setSpotInnerAngle(25);
	light->getComponent<Light>()->setRange(20);
	light->getComponent<Light>()->setIntensity(1);
	light->transform()->setPosition(QVector3D(5, 5, 5));
	light->transform()->lookAt(QVector3D(), Transform::up());

	light = Light::create(Light::Point);
	light->transform()->setPosition(QVector3D(-5, 1, -5));
	light->getComponent<Light>()->setColor(Qt::blue);
	light->getComponent<Light>()->setRange(10);

	light = Light::create(Light::Point);
	light->transform()->setPosition(QVector3D(-5, 1, 5));
	light->getComponent<Light>()->setColor(Qt::red);
	light->getComponent<Light>()->setRange(10);

	light = Light::create(Light::Point);
	light->transform()->setPosition(QVector3D(5, 1, -5));
	light->getComponent<Light>()->setColor(Qt::green);
	light->getComponent<Light>()->setRange(10);

	light = Light::create(Light::Point);
	light->transform()->setPosition(QVector3D(5, 1, 5));
	light->getComponent<Light>()->setColor(Qt::yellow);
	light->getComponent<Light>()->setRange(10);
	light->addComponent<LightController>();

	for (auto object : ProjectManager::instance()->activeProject()->getActiveScene()->gameObjects())
		printTransformTree(object->transform());

	if (Application::initialize(a, Settings()))
		Application::run();

	return a.exec();
}
