#include "primitive.h"
#include <cstdio>

Triangle3D::Triangle3D(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2, bool state_render) : m_v0(vertex0), m_v1(vertex1), m_v2(vertex2)
{
	Vector3f edg0 = m_v1.pos - m_v0.pos;
	Vector3f edg1 = m_v2.pos - m_v1.pos;

	m_vs[0] = m_v0;
	m_vs[1] = m_v1;
	m_vs[2] = m_v2;

	if (shader_mode & RENDER_MODE_FACE)
	{
		if (!state_render)
		{
			float normal = edg0.Cross(edg1).Normalize() * Eye.Normalize();
			if (normal < 0.0f)
			{
				face_render = false;
			}
		}
	}

	//环境光
	m_v0.color *= Intensity;
	m_v1.color *= Intensity;
	m_v2.color *= Intensity;
}

Triangle3D::Triangle3D(const Triangle3D &t3d) : m_v0(t3d.m_v0), m_v1(t3d.m_v1), m_v2(t3d.m_v2), face_render(t3d.face_render)
{
	m_vs[0] = m_v0;
	m_vs[1] = m_v1;
	m_vs[2] = m_v2;
}

Triangle3D &Triangle3D::operator=(const Triangle3D &t3d)
{
	m_v0 = t3d.m_v0;
	m_v1 = t3d.m_v1;
	m_v2 = t3d.m_v2;

	m_vs[0] = m_v0;
	m_vs[1] = m_v1;
	m_vs[2] = m_v2;

	Vector3f edg0 = m_v1.pos - m_v0.pos;
	Vector3f edg1 = m_v2.pos - m_v1.pos;

	face_render = t3d.face_render;

	return *this;
}

Triangle3D Triangle3D::operator*(const Matrix4f &m4f) const
{
	Vertex v0 = m_v0;
	Vertex v1 = m_v1;
	Vertex v2 = m_v2;
	v0.pos = m4f * m_v0.pos;
	v1.pos = m4f * m_v1.pos;
	v2.pos = m4f * m_v2.pos;
	return Triangle3D(v0, v1, v2);
}

void Triangle3D::Show()
{
	if (!face_render)
	{
		return;
	}

	m_v0.pos = m_v0.pos.To3D();
	m_v1.pos = m_v1.pos.To3D();
	m_v2.pos = m_v2.pos.To3D();

	Draw_Line(m_v0, m_v1);
	Draw_Line(m_v1, m_v2);
	Draw_Line(m_v2, m_v0);

	//如果为线框模式则不为多边形进行填充
	if (shader_mode & RENDER_MODE_LINE)
	{
		return;
	}

	//高洛德着色
	Raster();
}

void Triangle3D::Raster() const
{
	Vertex temp0, temp1, temp2;
	if (m_v0.pos.y != m_v1.pos.y && m_v1.pos.y != m_v2.pos.y && m_v0.pos.y != m_v2.pos.y)
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
		Triangle3D t0(temp0, temp1, new_vertex, true);
		Triangle3D t1(temp1, temp2, new_vertex, true);
		t0.Show();
		t1.Show();
		return;
	}
	if (m_v0.pos.y == m_v1.pos.y)
	{
		temp0 = m_v2;
		temp1 = m_v0;
		temp2 = m_v1;
	}
	else if (m_v1.pos.y == m_v2.pos.y)
	{
		temp0 = m_v0;
		temp1 = m_v1;
		temp2 = m_v2;
	}
	else if (m_v0.pos.y == m_v2.pos.y)
	{
		temp0 = m_v1;
		temp1 = m_v2;
		temp2 = m_v0;
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
		Shader_Line(ve, vs);
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

void Triangle3D::Draw_Line(const Vertex &v0, const Vertex &v1) const
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

void Triangle3D::Shader_Line(const Vertex &v0, const Vertex &v1) const
{
	float dx = v1.pos.x - v0.pos.x;
	float dz = v1.pos.z - v0.pos.z;
	float dm = COMPARE(dx, dz);

	float dr = v1.color.x - v0.color.x;
	float dg = v1.color.y - v0.color.y;
	float db = v1.color.z - v0.color.z;

	dx /= dm * POINT_DENSITY;
	dz /= dm * POINT_DENSITY;

	dr /= dm * POINT_DENSITY;
	dg /= dm * POINT_DENSITY;
	db /= dm * POINT_DENSITY;

	float x = v0.pos.x, y = v0.pos.y, z = v0.pos.z;
	float r = v0.color.x, g = v0.color.y, b = v0.color.z;
	float distance = dm * POINT_DENSITY;
	for (int i = 0; i < distance; i++)
	{
		if (x == z)
		{

		}
		x += dx;
		z += dz;

		r += dr;
		g += dg;
		b += db;

		glColor3f(r, g, b);
		glVertex3f(x, y, z);
	}
}

void Triangle3D::Triangle_Sort(Vertex &temp0, Vertex &temp1, Vertex &temp2) const
{
	if (m_v0.pos.y > m_v1.pos.y)
	{
		temp0 = m_v0;
		if (temp0.pos.y > m_v2.pos.y)
		{
			if (m_v2.pos.y > m_v1.pos.y)
			{
				temp1 = m_v2;
				temp2 = m_v1;
			}
			else
			{
				temp1 = m_v1;
				temp2 = m_v2;
			}
		}
		else
		{
			temp0 = m_v2;
			temp1 = m_v0;
			temp2 = m_v1;
		}
	}
	else
	{
		temp0 = m_v1;
		if (temp0.pos.y > m_v2.pos.y)
		{
			if (m_v0.pos.y > m_v2.pos.y)
			{
				temp1 = m_v0;
				temp2 = m_v2;
			}
			else
			{
				temp1 = m_v2;
				temp2 = m_v0;
			}
		}
		else
		{
			temp0 = m_v2;
			temp1 = m_v1;
			temp2 = m_v0;
		}
	}
}