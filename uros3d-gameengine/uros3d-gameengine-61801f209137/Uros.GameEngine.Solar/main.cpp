#include <ctime>
#include <QApplication>
#include <QColor>
#include <QVector3D>

#include "Application.h"
#include "ProjectManager.h"
#include "GameObject.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Debugger.h"
#include "Rendering/Renderer.h"

#include "FlyToAnimator.hpp"
#include "CameraController.hpp"
#include "SunGlow.hpp"
#include "RotateAround.hpp"

#define PLANET_TEXTURES_DIR "Textures\\Planets"
#define SKYBOX_TEXTURES_DIR "Textures\\SkyBox"

using namespace GameEngine;

struct Planet
{
	Planet(QString name, float sizeRatio, float orbitRadius, float rotationSpeed, float revolutionSpeed)
		: name(name),
		  sizeRatio(sizeRatio),
		  orbitRadius(orbitRadius),
		  rotationSpeed(rotationSpeed),
		  revolutionSpeed(revolutionSpeed) {}

	QString name;
	float sizeRatio;
	float orbitRadius;
	float rotationSpeed;
	float revolutionSpeed;
};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	auto project = new Project("Solar");
	auto scene = new Scene("MainScene");
	project->setActiveScene(scene);
	ProjectManager::instance()->loadProject(project);

	Light::setAmbientLightColor(QColor(6, 6, 6));
	auto light = Light::create(Light::Point);
	light->transform()->setPosition(QVector3D());
	light->getComponent<Light>()->setRange(1000);
	light->getComponent<Light>()->setIntensity(1.75);
	//light->addComponent<Debugger>()->hideOctreeNode();

	auto camera = Camera::create();
	camera->transform()->setPosition(QVector3D(-30, 0, 0));
	camera->transform()->lookAt(QVector3D());
	camera->addComponent<CameraController>();
	camera->addComponent<FlyToAnimator>()->disable();
	camera->getComponent<Camera>()->setSkyBox(
		      SkyBox(
			      Texture(SKYBOX_TEXTURES_DIR"\\top.jpg"),
			      Texture(SKYBOX_TEXTURES_DIR"\\bottom.jpg"),
			      Texture(SKYBOX_TEXTURES_DIR"\\front.jpg"),
			      Texture(SKYBOX_TEXTURES_DIR"\\back.jpg"),
			      Texture(SKYBOX_TEXTURES_DIR"\\left.jpg"),
			      Texture(SKYBOX_TEXTURES_DIR"\\right.jpg")
		      ));

	const float orbitRadius = 30;
	QHash<QString, Planet> planets =
		{
			{ "Mercury", Planet("Mercury", 1 / 2.54f, orbitRadius, 500, 50) },
			{ "Venus", Planet("Venus", 1 / 1.05f, orbitRadius * 2, 500, 30) },
			{ "Earth", Planet("Earth", 1.0f, orbitRadius * 3, 500, 10) },
			{ "Mars", Planet("Mars", 1 / 1.88f, orbitRadius * 4, 500, 5) },

			{ "Jupiter", Planet("Jupiter", 11.01f, orbitRadius * 7, 200, 3) },
			{ "Saturn", Planet("Saturn", 9.41f, orbitRadius * 10, 100, 3) },
			{ "Uranus", Planet("Uranus", 4.00f, orbitRadius * 13, 100, 3) },
			{ "Neptune", Planet("Neptune", 3.88f, orbitRadius * 16, 100, 3) }
		};

	const float sunRadius = 20;
	auto sun = GameObject::createPrimitive(GameObject::Sphere, QVector3D());
	sun->setName("Sun");
	sun->transform()->scale(sunRadius, sunRadius, sunRadius);
	auto rotateAround = sun->addComponent<RotateAround>();
	rotateAround->target = sun->transform();
	rotateAround->speed = 10.0f;
	auto& sunMaterial = sun->getComponent<Renderer>()->getMaterial();
	sunMaterial.setTexture(Texture(PLANET_TEXTURES_DIR"\\sun.jpg"));
	sunMaterial.setShaderType(Material::Unlit);

	auto plane = GameObject::createPrimitive(GameObject::Plane, QVector3D());
	plane->setName("SunGlow");
	plane->transform()->scale(sunRadius * 1.2, sunRadius * 1.2, sunRadius * 1.2);
	plane->getComponent<Renderer>()->getMaterial().setShaderType(Material::UnlitTransparent);
	plane->getComponent<Renderer>()->getMaterial().setTexture(Texture(PLANET_TEXTURES_DIR"\\glow.png"));
	plane->transform()->setParent(sun->transform());
	plane->addComponent<SunGlow>()->camera = camera->getComponent<Camera>();

	srand(time(nullptr));
	for (auto planet : planets)
	{
		float r = rand();
		auto planetObject = GameObject::createPrimitive(GameObject::Sphere, QVector3D(cos(r) * planet.orbitRadius, 0, sin(r) * planet.orbitRadius));
		planetObject->setName(planet.name);
		planetObject->transform()->scale(planet.sizeRatio, planet.sizeRatio, planet.sizeRatio);
		planetObject->getComponent<Renderer>()->getMaterial().setTexture(Texture(PLANET_TEXTURES_DIR"\\" + planet.name.toLower() + ".jpg"));
		rotateAround = planetObject->addComponent<RotateAround>();
		rotateAround->target = sun->transform();
		rotateAround->speed = planet.revolutionSpeed;
		rotateAround->selfRotationSpeed = planet.rotationSpeed;
	}

	auto moon = GameObject::createPrimitive(GameObject::Sphere);
	moon->setName("Moon");
	moon->transform()->scale(0.3f, 0.3f, 0.3f);
	moon->transform()->setParent(scene->findGameObject("Earth")->transform());
	moon->transform()->setLocalPosition(QVector3D(3, 0, 0));
	moon->getComponent<Renderer>()->getMaterial().setTexture(Texture(PLANET_TEXTURES_DIR"\\moon.jpg"));
	rotateAround = moon->addComponent<RotateAround>();
	rotateAround->speed = 1000;
	rotateAround->target = moon->transform();

	auto vantage = new GameObject("VantagePoint");
	vantage->transform()->setPosition(QVector3D(0, 100, 0));

	auto settings = Settings();
	settings.disableVSync();
	settings.setWindowType(Settings::Window);
	settings.setAntialiasingType(Settings::MSAAx8);
	if (Application::initialize(a, settings))
		Application::run();

	return a.exec();
}
