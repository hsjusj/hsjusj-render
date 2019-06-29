#ifndef PRIMITIVE_H__

#define PRIMITIVE_H__

#include <conio.h>
#include <math.h>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include "math_3d.h"
#include "state_machine.h"

#include "main.h"

#define COMPARE(x, y) (abs(x)) > (abs(y)) ? (abs(x)) : (abs(y))

class Triangle2D
{
public:
	Triangle2D(const Vector2f&, const Vector2f&, const Vector2f&);
	void Show();
private:
	Vector2f v0, v1, v2;
	void Draw_Line(const Vector2f&, const Vector2f&);
	void Raster();
};

class Triangle3D
{
	friend class RenderList;
public:
	Triangle3D() = default;
	Triangle3D(const Vertex&, const Vertex&, const Vertex&, bool = false);
	Triangle3D(const Triangle3D&);
	Triangle3D &operator=(const Triangle3D&);
	Triangle3D operator*(const Matrix4f&) const;
	void Show();
private:
	Vertex m_vs[3];
	Vertex m_v0, m_v1, m_v2;
	bool face_render = true;
	void Triangle_Sort(Vertex&, Vertex&, Vertex&) const;
	void Raster() const;
	void Draw_Line(const Vertex&, const Vertex&) const;
	//����ɨ����ɫ���Ż�������ȡ��Draw_Line(...)����
	//����ûʲô�ã�������������ʱʹ������
	void Shader_Line(const Vertex&, const Vertex&) const;
};

#endif