#include <QApplication>
#include <QColor>
#include <QVector3D>

#include "Application.h"
#include "ProjectManager.h"
#include "GameObject.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Rendering/Renderer.h"

#include "CameraController.hpp"
#include "RotateAround.hpp"

using namespace GameEngine;

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	auto project = new Project("CullingTests");
	auto scene = new Scene("MainScene");
	project->setActiveScene(scene);
	ProjectManager::instance()->loadProject(project);

	const int size = 100;
	QColor diffuse(rand() % 256, rand() % 256, rand() % 256);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			auto go = GameObject::createPrimitive(GameObject::Sphere, QVector3D((-size / 2 + i) * 2, 0, (-size / 2 + j) * 2));
			go->markAsStatic();
			go->getComponent<Renderer>()->getMaterial().setDiffuseColor(diffuse);
			//go->getComponent<Renderer>()->getMaterial().setShaderType(Material::StandardTransparent);
			//go->getComponent<Renderer>()->getMaterial().setOpacity(0.5);
			if (i % (size / 4) == 0 && j % (size / 4) == 0)
				diffuse = QColor(rand() % 256, rand() % 256, rand() % 256);
		}

	Light::setAmbientLightColor(QColor(10, 10, 10));
	auto light = Light::create(Light::Directional);
	light->transform()->setPosition(QVector3D(0, 10, 0));
	light->transform()->setForward(QVector3D(0, -1, 0));

	auto camera = Camera::create();
	camera->transform()->setPosition(QVector3D(-10, 0, 0));
	camera->transform()->lookAt(QVector3D());
	camera->addComponent<CameraController>();

	auto cullingCamera = Camera::create();
	cullingCamera->transform()->lookAt(QVector3D(1, 0, 0));
	cullingCamera->addComponent<RotateAround>();

	if (Application::initialize(a, Settings()))
		Application::run();

	return a.exec();
}
