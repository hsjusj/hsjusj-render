#include "math_3d.h"

Vertex::Vertex() : color(1.0f, 1.0f, 1.0f)
{

}

Vertex::Vertex(const Vertex &v0) : pos(v0.pos), color(v0.color), normal(v0.normal)
{

}

Vertex::Vertex(const Vector3f &v0) : pos(v0)
{

}

Vertex::Vertex(const Vector3f &v0, const Vector3f &c0) : pos(v0), color(c0)
{

}

Vertex &Vertex::operator=(const Vertex &v0)
{
	pos = v0.pos;
	color = v0.color;
	normal = v0.normal;
	return *this;
}