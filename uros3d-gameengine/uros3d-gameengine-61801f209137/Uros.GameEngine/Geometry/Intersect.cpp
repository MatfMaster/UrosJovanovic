#include "Intersect.h"
#include "Ray3D.h"
#include "BoundingBox.h"
#include "Scene/Camera.h"

GameEngine::Intersect::Intersect() {}

GameEngine::Intersect::~Intersect() {}

bool GameEngine::Intersect::aabbAndAABB(const BoundingBox& box1, const BoundingBox& box2)
{
	return BoundingBox::isInsideOf(box1, box2) || BoundingBox::isInsideOf(box2, box1) || BoundingBox::intersect(box1, box2);
}

bool GameEngine::Intersect::triangleAndAABB(const QVector3D& a, const QVector3D& b, const QVector3D& c, const BoundingBox& box)
{
	// Code ported from C# example at
	// http://stackoverflow.com/questions/17458562/efficient-aabb-triangle-intersection-in-c-sharp

	float triangleMin, triangleMax;
	float boxMin, boxMax;

	// Test the box normals (x-, y- and z-axes)
	QVector<QVector3D> boxNormals =
		{
			QVector3D(1, 0, 0),
			QVector3D(0, 1, 0),
			QVector3D(0, 0, 1)
		};
	QVector<QVector3D> triangleVertices = { a, b, c };

	for (int i = 0; i < 3; i++)
	{
		project(triangleVertices, boxNormals[i], &triangleMin, &triangleMax);
		if (triangleMax < box.minPoint()[i] || triangleMin > box.maxPoint()[i])
			return false; // No intersection possible.
	}

	QVector<QVector3D> boxVertices(8);
	box.getVertices(boxVertices);

	// Test the triangle normal
	QVector3D triangleNormal = QVector3D::normal(c - a, b - a);
	float triangleOffset = QVector3D::dotProduct(triangleNormal, a);
	project(boxVertices, triangleNormal, &boxMin, &boxMax);
	if (boxMax < triangleOffset || boxMin > triangleOffset)
		return false; // No intersection possible.

	// Test the nine edge cross-products
	QVector<QVector3D> triangleEdges =
		{
			a - b,
			b - c,
			c - a
		};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			// The box normals are the same as it's edge tangents
			auto axis = QVector3D::crossProduct(triangleEdges[i], boxNormals[j]);
			project(boxVertices, axis, &boxMin, &boxMax);
			project(triangleVertices, axis, &triangleMin, &triangleMax);
			if (boxMax < /*=*/ triangleMin || boxMin > /*=*/ triangleMax)
				return false; // No intersection possible
		}

	// No separating axis found.
	return true;
}

bool GameEngine::Intersect::frustumAndAABB(const CameraFrustum& frustum, const BoundingBox& box)
{
	// From Geometric Approach - Testing Boxes II at
	// http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html

	QVector3D n, p;
	QVector<Plane3D> planes(6);

	frustum.getPlanes(planes);
	auto min = box.minPoint();
	auto max = box.maxPoint();

	for (int i = 0; i < 6; i++)
	{
		// X axis 
		if (planes[i].normal().x() > 0)
		{
			n.setX(min.x());
			p.setX(max.x());
		}
		else
		{
			n.setX(max.x());
			p.setX(min.x());
		}
		// Y axis 
		if (planes[i].normal().y() > 0)
		{
			n.setY(min.y());
			p.setY(max.y());
		}
		else
		{
			n.setY(max.y());
			p.setY(min.y());
		}
		// Z axis 
		if (planes[i].normal().z() > 0)
		{
			n.setZ(min.z());
			p.setZ(max.z());
		}
		else
		{
			n.setZ(max.z());
			p.setZ(min.z());
		}

		if (planes[i].signedDistanceToPoint(n) > 0)
			return false;
	}
	return true;
}

bool GameEngine::Intersect::rayAndAABB(const Ray3D& ray, const BoundingBox& box, float* t)
{
	// From "A Minimal Ray-Tracer: Rendering Simple Shapes" at
	// http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

	float tmin = (box.minPoint().x() - ray.origin().x()) / ray.direction().x();
	float tmax = (box.maxPoint().x() - ray.origin().x()) / ray.direction().x();

	if (tmin > tmax)
		std::swap(tmin, tmax);

	float tymin = (box.minPoint().y() - ray.origin().y()) / ray.direction().y();
	float tymax = (box.maxPoint().y() - ray.origin().y()) / ray.direction().y();

	if (tymin > tymax)
		std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (box.minPoint().z() - ray.origin().z()) / ray.direction().z();
	float tzmax = (box.maxPoint().z() - ray.origin().z()) / ray.direction().z();

	if (tzmin > tzmax)
		std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	*t = tmin;

	return true;
}

bool GameEngine::Intersect::rayAndTriangle(const Ray3D& ray, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, float* t)
{
	// From Fast Minimum Storage RayTriangle Intersection at
	// http://www.cs.virginia.edu/~gfx/courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
	// and here http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/

	QVector3D e1, e2, h, s, q;
	QVector3D p = ray.origin();
	QVector3D d = ray.direction();
	const float eps = 0.00001f;
	float a, f, u, v;

	e1 = v1 - v0;
	e2 = v2 - v0;

	h = QVector3D::crossProduct(d, e2);
	a = QVector3D::dotProduct(e1, h);

	if (a > -eps && a < eps)
		return false;

	f = 1 / a;
	s = p - v0;
	u = f * QVector3D::dotProduct(s, h);

	if (u < 0.0 || u > 1.0)
		return false;

	q = QVector3D::crossProduct(s, e1);
	v = f * QVector3D::dotProduct(d, q);

	if (v < 0.0 || u + v > 1.0)
		return false;

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	*t = f * QVector3D::dotProduct(e2, q);

	if (*t > eps) // ray intersection
		return true;

	else // this means that there is a line intersection
	// but not a ray intersection
		return false;
}

void GameEngine::Intersect::project(const QVector<QVector3D>& points, const QVector3D& axis, float* min, float* max)
{
	*min = std::numeric_limits<float>::max();
	*max = std::numeric_limits<float>::lowest();
	for (int i = 0; i < points.size(); i++)
	{
		float val = QVector3D::dotProduct(axis, points[i]);
		if (val < *min) *min = val;
		if (val > *max) *max = val;
	}
}
