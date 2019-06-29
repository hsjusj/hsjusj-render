#include "primitive.h"
#include <cstdio>

Triangle3D::Triangle3D(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2, bool state_render) : v0(vertex0), v1(vertex1), v2(vertex2)
{
	Vector3f edg0 = v1.pos - v0.pos;
	Vector3f edg1 = v2.pos - v1.pos;

	if (shader_mode & HS_FACE)
	{
		if (!state_render)
		{
			float normal = edg0.Cross(edg1) * Eye;
			if (normal <= 0.8f)
			{
				face_render = false;
			}
		}
	}

	v0.pos = vertex0.pos.To3D();
	v1.pos = vertex1.pos.To3D();
	v2.pos = vertex2.pos.To3D();
}

Triangle3D::Triangle3D(const Triangle3D &t3d) : v0(t3d.v0), v1(t3d.v1), v2(t3d.v2), face_render(t3d.face_render)
{

}

Triangle3D &Triangle3D::operator=(const Triangle3D &t3d)
{
	v0 = t3d.v0;
	v1 = t3d.v1;
	v2 = t3d.v2;

	Vector3f edg0 = v1.pos - v0.pos;
	Vector3f edg1 = v2.pos - v1.pos;

	face_render = t3d.face_render;

	return *this;
}

void Triangle3D::Show()
{
	if (!face_render)
	{
		return;
	}

	Draw_Line(v0, v1);
	Draw_Line(v1, v2);
	Draw_Line(v2, v0);

	//如果为线框模式则不为多边形进行填充
	if (shader_mode & HS_LINE)
	{
		return;
	}

	//高洛德着色
	Raster();
}

void Triangle3D::Raster()
{
	Vertex temp0, temp1, temp2;
	//printf("%f %f %f\n", v0.pos.y, v1.pos.y, v2.pos.y);
	//system("pause");
	if (v0.pos.y != v1.pos.y && v1.pos.y != v2.pos.y && v0.pos.y != v2.pos.y)
	{
		//三角形排序
		Triangle_Sort(temp0, temp1, temp2);
		Vertex new_vertex;
		
		float dy = temp2.pos.y - temp0.pos.y;
		float t = (temp1.pos.y - temp0.pos.y) / dy;

		new_vertex.pos.x = temp0.pos.x + (temp2.pos.x - temp0.pos.x) * t;
		new_vertex.pos.y = temp1.pos.y;
		new_vertex.pos.z = temp0.pos.z + (temp2.pos.z - temp0.pos.z) * t;

		new_vertex.color.x = temp0.color.x + (temp2.color.x - temp0.color.x) * t;
		new_vertex.color.y = temp0.color.y + (temp2.color.y - temp0.color.y) * t;
		new_vertex.color.z = temp0.color.z + (temp2.color.z - temp0.color.z) * t;
		//new_vertex.pos.w = 1.0f;
		//printf("%f\n", temp2.pos.w);
		Triangle3D t0(temp0, temp1, new_vertex, true);
		Triangle3D t1(temp1, temp2, new_vertex, true);
		t0.Show();
		t1.Show();
		return;
	}
	if (v0.pos.y == v1.pos.y)
	{
		temp0 = v2;
		temp1 = v0;
		temp2 = v1;
	}
	else if (v1.pos.y == v2.pos.y)
	{
		temp0 = v0;
		temp1 = v1;
		temp2 = v2;
	}
	else if (v0.pos.y == v2.pos.y)
	{
		temp0 = v1;
		temp1 = v2;
		temp2 = v0;
	}

	Vertex v0(temp0), v1(temp1), v2(temp2);

	if (v1.pos.x > v2.pos.x)
	{
		Vertex temp = v1;
		v1 = v2;
		v2 = temp;
	}

	Vertex ve(v0), vs(v0);

	float left_dx = v1.pos.x - v0.pos.x;
	float left_dz = v1.pos.z - v0.pos.z;
	float left_dr = v1.color.x - v0.color.x;
	float left_dg = v1.color.y - v0.color.y;
	float left_db = v1.color.z - v0.color.z;

	float right_dx = v2.pos.x - v0.pos.x;
	float right_dz = v2.pos.z - v0.pos.z;
	float right_dr = v2.color.x - v0.color.x;
	float right_dg = v2.color.y - v0.color.y;
	float right_db = v2.color.z - v0.color.z;

	float dy = abs(v0.pos.y - v1.pos.y);

	left_dx /= dy * POINT_DENSITY;
	left_dz /= dy * POINT_DENSITY;
	left_dr /= dy * POINT_DENSITY;
	left_dg /= dy * POINT_DENSITY;
	left_db /= dy * POINT_DENSITY;

	right_dx /= dy * POINT_DENSITY;
	right_dz /= dy * POINT_DENSITY;
	right_dr /= dy * POINT_DENSITY;
	right_dg /= dy * POINT_DENSITY;
	right_db /= dy * POINT_DENSITY;

	dy = v0.pos.y > v1.pos.y ? -1.0f / POINT_DENSITY : 1.0f / POINT_DENSITY;
	float Distance = abs(v0.pos.y - v1.pos.y) * POINT_DENSITY;
	for (int i = 0; i < Distance; i++)
	{
		Draw_Line(ve, vs);
		ve.pos.x += left_dx;
		ve.pos.y += dy;
		ve.pos.z += left_dz;
		ve.color.x += left_dr;
		ve.color.y += left_dg;
		ve.color.z += left_db;

		vs.pos.x += right_dx;
		vs.pos.y += dy;
		vs.pos.z += right_dz;
		vs.color.x += right_dr;
		vs.color.y += right_dg;
		vs.color.z += right_db;
	}
}

void Triangle3D::Draw_Line(const Vertex &v0, const Vertex &v1)
{

	float dx = v1.pos.x - v0.pos.x;
	float dy = v1.pos.y - v0.pos.y;
	float dz = v1.pos.z - v0.pos.z;
	float dm = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	dm = dm > abs(dz) ? dm : abs(dz);

	float dr = v1.color.x - v0.color.x;
	float dg = v1.color.y - v0.color.y;
	float db = v1.color.z - v0.color.z;

	dx /= dm * POINT_DENSITY;
	dy /= dm * POINT_DENSITY;
	dz /= dm * POINT_DENSITY;

	dr /= dm * POINT_DENSITY;
	dg /= dm * POINT_DENSITY;
	db /= dm * POINT_DENSITY;

	float x = v0.pos.x,   y = v0.pos.y,   z = v0.pos.z;
	float r = v0.color.x, g = v0.color.y, b = v0.color.z;
	float distance = dm * POINT_DENSITY;
	for (int i = 0; i < distance; i++)
	{
		glColor3f(r, g, b);
		glVertex3f(x, y, z);

		x += dx;
		y += dy;
		z += dz;

		r += dr;
		g += dg;
		b += db;
	}
}

void Triangle3D::Triangle_Sort(Vertex &temp0, Vertex &temp1, Vertex &temp2)
{
	if (v0.pos.y > v1.pos.y)
	{
		temp0 = v0;
		if (temp0.pos.y > v2.pos.y)
		{
			if (v2.pos.y > v1.pos.y)
			{
				temp1 = v2;
				temp2 = v1;
			}
			else
			{
				temp1 = v1;
				temp2 = v2;
			}
		}
		else
		{
			temp0 = v2;
			temp1 = v0;
			temp2 = v1;
		}
	}
	else
	{
		temp0 = v1;
		if (temp0.pos.y > v2.pos.y)
		{
			if (v0.pos.y > v2.pos.y)
			{
				temp1 = v0;
				temp2 = v2;
			}
			else
			{
				temp1 = v2;
				temp2 = v0;
			}
		}
		else
		{
			temp0 = v2;
			temp1 = v1;
			temp2 = v0;
		}
	}
}