#include "math_3d.h"

Vertex::Vertex() : color(1.0f, 1.0f, 1.0f)
{

}

Vertex::Vertex(const Vertex &v0) : pos(v0.pos), color(v0.color), normal(v0.normal)
{

}

Vertex::Vertex(const Vector3f &pos0) : pos(pos0), color(Vector3f(1.0f, 1.0f, 1.0f))
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

static float Fast_SquareRT(float f)
{
	float result;
	_asm
	{
		mov eax, f
			sub eax, 0x3f800000
			sar eax, 1
			add eax, 0x3f800000
			mov result, eax
	}
	return result;
}