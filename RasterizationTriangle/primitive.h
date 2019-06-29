#ifndef PRIMITIVE_H__

#define PRIMITIVE_H__

#include <math.h>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include "math_3d.h"
#include "main.h"
#include "state_machine.h"

class Triangle2D
{
public:
	Triangle2D(const Vector2f&, const Vector2f&, const Vector2f&);
	void Show();
private:
	Vector2f v0, v1, v2;
	void Raster();
};

class Triangle3D
{
public:
	Triangle3D(const Vertex&, const Vertex&, const Vertex&);
	void Show();
private:
	Vertex v0, v1, v2;
	void Triangle_Sort(Vertex&, Vertex&, Vertex&);
	void Raster();
	void Draw_Line(const Vertex&, const Vertex&);
};

#endif