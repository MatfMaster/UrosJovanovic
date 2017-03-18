#include <QtCore/QCoreApplication>
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Rendering/Material.h"
#include "Geometry/Plane3D.h"

#define EPS 1e-3
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

using namespace GameEngine;

namespace GameEngine {
	namespace Tests {
		bool equalsApproximately(const QVector3D& vec1, const QVector3D& vec2, const float& epsilon = EPS)
		{
			return fabs(vec1.x() - vec2.x()) <= epsilon
				&& fabs(vec1.y() - vec2.y()) <= epsilon
				&& fabs(vec1.z() - vec2.z()) <= epsilon;
		}

		TEST_CASE("GameObject")
		{
			class TestComponentA : public Component
			{
			public:
				explicit TestComponentA(GameObject* gameObject)
					: Component(gameObject) { }
				ComponentType type() const override { return T_TEST; }
			};

			class TestComponentB : public Component
			{
			public:
				explicit TestComponentB(GameObject* gameObject)
					: Component(gameObject) { }
				ComponentType type() const override { return T_TEST; }
			};

			class TestComponentC : public Component
			{
			public:
				explicit TestComponentC(GameObject* gameObject)
					: Component(gameObject) { }
				ComponentType type() const override { return T_TEST; }
			};

			auto gameObject = new GameObject("MyGameObject");

			gameObject->addComponent<TestComponentA>();
			gameObject->addComponent<TestComponentB>();
			gameObject->addComponent<TestComponentC>();
			gameObject->addComponent<TestComponentA>();
			REQUIRE(gameObject->getComponents().count() == 5) ; //We already have transform
			REQUIRE(gameObject->getComponent<TestComponentA>()) ;
			REQUIRE(gameObject->getComponents<TestComponentA>().count() == 2) ;

			for (auto component : gameObject->getComponents().toVector())
				Component::destroy(component);
			REQUIRE(gameObject->getComponents().count() == 0) ;
			REQUIRE(!gameObject->getComponent<TestComponentB>()) ;
			REQUIRE(gameObject->getComponents<TestComponentC>().count() == 0) ;

			GameObject::destroy(gameObject);
		}

		TEST_CASE("Transform-Rotation")
		{
			auto gameObject = new GameObject("MyGameObject");
			auto transform = gameObject->transform();
			REQUIRE(transform == gameObject->getComponent<Transform>()) ;

			transform->rotate(10, 90, 1);
			REQUIRE(equalsApproximately(transform->getEulerAngles(), QVector3D(10, 90, 1))) ;
			REQUIRE(equalsApproximately(transform->getPosition(), QVector3D(0, 0, 0))) ;

			transform->setEulerAngles(QVector3D(0, 0, 0));
			REQUIRE(equalsApproximately(transform->getEulerAngles(), QVector3D(0, 0, 0))) ;
			REQUIRE(equalsApproximately(transform->getPosition(), QVector3D(0, 0, 0))) ;

			transform->rotate(QVector3D(0, 1, 0), 30);
			REQUIRE(equalsApproximately(transform->getEulerAngles(), QVector3D(0, 30, 0))) ;
			REQUIRE(equalsApproximately(transform->getPosition(), QVector3D(0, 0, 0))) ;

			transform->rotate(QVector3D(0, 1, 0), -30);
			REQUIRE(equalsApproximately(transform->getEulerAngles(), QVector3D(0, 0, 0))) ;
			REQUIRE(equalsApproximately(transform->getPosition(), QVector3D(0, 0, 0))) ;

			transform->rotateAround(QVector3D(1, 0, 0), QVector3D(0, 1, 0), 180);
			REQUIRE(fabs(fabs(transform->getEulerAngles().y()) - 180) <= EPS) ;
			REQUIRE(equalsApproximately(transform->getPosition(), QVector3D(2, 0, 0))) ;

			transform->setRotation(QQuaternion()); //Reset rotation
			transform->setPosition(QVector3D(1, 0, 0));
			transform->setUp(QVector3D(-1, 0, 0));
			REQUIRE(equalsApproximately(transform->getUp(), QVector3D(-1, 0, 0))) ;
			REQUIRE(equalsApproximately(transform->getEulerAngles(), QVector3D(0, 0, 90))) ;
			REQUIRE(equalsApproximately(transform->getRight(), QVector3D(0, 1, 0))) ;
			REQUIRE(equalsApproximately(transform->getForward(), QVector3D(0, 0, 1))) ;

			GameObject::destroy(gameObject);
		}

		TEST_CASE("Transform-Hierarchy")
		{
			auto goA = new GameObject("A");
			auto goB = new GameObject("B");
			auto goC = new GameObject("C");

			auto transformA = goA->getComponent<Transform>();
			auto transformB = goB->getComponent<Transform>();
			auto transformC = goC->getComponent<Transform>();

			transformB->setParent(transformA);
			transformC->setParent(transformA);
			REQUIRE(transformA->children().count() == 2) ;

			transformC->setParent(transformB);
			REQUIRE(transformA->children().count() == 1) ;
			REQUIRE((transformB->children().first()) == transformC) ;

			//This should fail cause C is lower than B
			transformB->setParent(transformC);
			REQUIRE(transformB->getParent() != transformC) ;

			transformA->removeChildRecursively(transformC);
			//transformC->setParent(nullptr);
			transformB->setParent(transformC);
			transformA->setParent(transformB);
			REQUIRE(transformA->getParent() == transformB) ;
			REQUIRE(transformB->getParent() == transformC) ;
			REQUIRE(transformC->children().count() == 1) ;
			REQUIRE(transformB->children().count() == 1) ;
			REQUIRE(transformC->children().first()->children().first() == transformA) ;

			GameObject::destroy(goC); //this will destroy A and B also
		}

		TEST_CASE("Transform-Space")
		{
			auto goA = new GameObject("A");
			auto goB = new GameObject("B");

			goA->transform()->addChild(goB->transform());
			goB->transform()->setLocalPosition(QVector3D(1, 0, 0));
			REQUIRE(equalsApproximately(goB->transform()->getLocalPosition(), QVector3D(1, 0, 0))) ;

			goA->transform()->translate(QVector3D(1, 0, 0), Transform::World);
			REQUIRE(equalsApproximately(goB->transform()->getLocalPosition(), QVector3D(1, 0, 0))) ;
			REQUIRE(equalsApproximately(goB->transform()->getPosition(), QVector3D(2, 0, 0))) ;

			goB->transform()->translate(QVector3D(1, 0, 0), Transform::Self);
			REQUIRE(equalsApproximately(goA->transform()->getPosition(), QVector3D(1, 0, 0))) ; //Parent position must not change
			REQUIRE(equalsApproximately(goB->transform()->getPosition(), QVector3D(3, 0, 0))) ;

			goA->transform()->rotateAround(QVector3D(0, 0, 0), Transform::up(), 90);
			REQUIRE(equalsApproximately(goA->transform()->getEulerAngles(), QVector3D(0, 90, 0))) ;
			REQUIRE(equalsApproximately(goA->transform()->getPosition(), QVector3D(0, 0, -1))) ;
			REQUIRE(equalsApproximately(goB->transform()->getPosition(), QVector3D(0, 0, -3))) ;
			REQUIRE(equalsApproximately(goB->transform()->getLocalPosition(), QVector3D(0, 0, -2))) ;

			goB->transform()->setLocalScale(QVector3D(0.5, 0.5, 0.5));
			goA->transform()->setLocalScale(QVector3D(2, 2, 2));
			REQUIRE(equalsApproximately(goB->transform()->getLocalScale(), QVector3D(0.5, 0.5, 0.5))) ;
			REQUIRE(equalsApproximately(goB->transform()->getScale(), QVector3D(1, 1, 1))) ;

			GameObject::destroy(goA);
		}

		TEST_CASE("Material")
		{
			Material m1, m2;
			REQUIRE(m1 == m2) ;
			m1.setShaderType(Material::Unlit);
			REQUIRE(m1 != m2) ;
			m2.setShaderType(Material::Unlit);
			REQUIRE(m1 == m2) ;
			m1.setDiffuseColor(Qt::green);
			m2.setDiffuseColor(Qt::blue);
			REQUIRE(m1 != m2) ;
			m1 = m2;
			REQUIRE(m1 == m2) ;
			m1.setSpecularColor(Qt::green);
			m2.setSpecularColor(Qt::blue);
			REQUIRE(m1 != m2) ;
			m1 = m2;
			REQUIRE(m1 == m2) ;
			m1.setShininess(1);
			m2.setShininess(2);
			REQUIRE(m1 != m2) ;
			m2.setShaderType(Material::UnlitTransparent);
			REQUIRE(m1 < m2) ;
		}

		TEST_CASE("Math")
		{
			auto plane = Plane3D(QVector3D(0, 1, 0), 1);	
			REQUIRE(plane.relationToPoint(QVector3D(0, 2, 0)) == Plane3D::InFrontOfPlane);
			REQUIRE(plane.relationToPoint(QVector3D(0, -2, 0)) == Plane3D::BehindThePlane);
			REQUIRE(plane.relationToPoint(QVector3D(0, 1, 0)) == Plane3D::OnPlane);

			plane = Plane3D(QVector3D(0, 1, 0), QVector3D(1, 1, 0), QVector3D(0, 1, 1));
			REQUIRE(plane.relationToPoint(QVector3D(0, 2, 0)) == Plane3D::InFrontOfPlane);
			REQUIRE(plane.relationToPoint(QVector3D(0, -2, 0)) == Plane3D::BehindThePlane);
			REQUIRE(plane.relationToPoint(QVector3D(0, 1, 0)) == Plane3D::OnPlane);
		}
	}
}
