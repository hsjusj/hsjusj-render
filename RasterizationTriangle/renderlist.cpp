#include "renderlist.h"

using std::vector;

RenderList::RenderList()
{

}

//先对三角形所在位置进行判断，是否进行剔除或者近平面裁剪
void RenderList::AddTriangle(const Triangle3D &t3d)
{
	vector<Vertex> new_vertexs;
	vector<Vertex> out_vertexs;
	vector<Vertex> in_vertexs;

	Triangle3D spaceTriangle3D = t3d * GamePipeline.GetProjTrans();

	Vector3f v0 = spaceTriangle3D.m_vs[0].pos.To3D();
	Vector3f v1 = spaceTriangle3D.m_vs[1].pos.To3D();
	Vector3f v2 = spaceTriangle3D.m_vs[2].pos.To3D();

	if (abs(v0.x) > 1.0f && abs(v1.x) > 1.0f && abs(v2.x) > 1.0f)
	{
		return;
	}
	else if (abs(v0.y) > 1.0f && abs(v1.y) > 1.0f && abs(v2.y) > 1.0f)
	{
		return;
	}
	//需要裁剪
	else if (t3d.m_vs[0].pos.z < ZNEAR || t3d.m_vs[1].pos.z < ZNEAR || t3d.m_vs[2].pos.z < ZNEAR)
	{
		for (int i = 0; i < 3; i++)
		{
			if (t3d.m_vs[i].pos.z < ZNEAR)
			{
				out_vertexs.push_back(t3d.m_vs[i]);
			}
			else
			{
				in_vertexs.push_back(t3d.m_vs[i]);
			}
		}
		if (out_vertexs.size() == 3)
		{
			//3个顶点在外面，全部剔除
			return;
		}
		//否则，裁剪

		float v0z = t3d.m_vs[0].pos.z, v1z = t3d.m_vs[1].pos.z, v2z = t3d.m_vs[2].pos.z;
		float dz0 = v0z - v1z;
		float t0 = (ZNEAR - v1z) / dz0;

		float dz1 = v1z - v2z;
		float t1 = (ZNEAR - v2z) / dz1;

		float dz2 = v2z - v0z;
		float t2 = (ZNEAR - v0z) / dz2;
		//printf("t0:%f t1:%f t2:%f\n", t0, t1, t2);
		
		//if (abs(t0) > 1.0f && abs(t1) > 1.0f && abs(t2) > 1.0f)
		//{
		//	printf("丢弃");
		//	return;
		//}
		
		if (abs(t0) <= 1.0f)
		{
			float dx = t3d.m_vs[0].pos.x - t3d.m_vs[1].pos.x;
			float dy = t3d.m_vs[0].pos.y - t3d.m_vs[1].pos.y;
			float dr = t3d.m_vs[0].color.x - t3d.m_vs[1].color.x;
			float dg = t3d.m_vs[0].color.y - t3d.m_vs[1].color.y;
			float db = t3d.m_vs[0].color.z - t3d.m_vs[1].color.z;

			float x = t3d.m_vs[1].pos.x + t0 * dx;
			float y = t3d.m_vs[1].pos.y + t0 * dy;
			float r = t3d.m_vs[1].color.x + t0 * dr;
			float g = t3d.m_vs[1].color.y + t0 * dg;
			float b = t3d.m_vs[1].color.z + t0 * db;
			Vertex new_v(Vector3f(x, y, ZNEAR), Vector3f(r, g, b));
			new_vertexs.push_back(new_v);
		}
		if (abs(t1) <= 1.0f)
		{
			float dx = t3d.m_vs[1].pos.x - t3d.m_vs[2].pos.x;
			float dy = t3d.m_vs[1].pos.y - t3d.m_vs[2].pos.y;
			float dr = t3d.m_vs[1].color.x - t3d.m_vs[2].color.x;
			float dg = t3d.m_vs[1].color.y - t3d.m_vs[2].color.y;
			float db = t3d.m_vs[1].color.z - t3d.m_vs[2].color.z;

			float x = t3d.m_vs[2].pos.x + t1 * dx;
			float y = t3d.m_vs[2].pos.y + t1 * dy;
			float r = t3d.m_vs[2].color.x + t1 * dr;
			float g = t3d.m_vs[2].color.y + t1 * dg;
			float b = t3d.m_vs[2].color.z + t1 * db;

			Vertex new_v(Vector3f(x, y, ZNEAR), Vector3f(r, g, b));
			new_vertexs.push_back(new_v);
		}
		if (abs(t2) <= 1.0f)
		{
			float dx = t3d.m_vs[2].pos.x - t3d.m_vs[0].pos.x;
			float dy = t3d.m_vs[2].pos.y - t3d.m_vs[0].pos.y;
			float dr = t3d.m_vs[2].color.x - t3d.m_vs[0].color.x;
			float dg = t3d.m_vs[2].color.y - t3d.m_vs[0].color.y;
			float db = t3d.m_vs[2].color.z - t3d.m_vs[0].color.z;

			float x = t3d.m_vs[0].pos.x + t2 * dx;
			float y = t3d.m_vs[0].pos.y + t2 * dy;
			float r = t3d.m_vs[0].color.x + t2 * dr;
			float g = t3d.m_vs[0].color.y + t2 * dg;
			float b = t3d.m_vs[0].color.z + t2 * db;

			Vertex new_v(Vector3f(x, y, ZNEAR), Vector3f(r, g, b));
			new_vertexs.push_back(new_v);
		}

		if (in_vertexs.size() == 1)
		{
			if (new_vertexs[0].pos.x < new_vertexs[1].pos.x)
			{
				Triangle3D new_t3d(in_vertexs[0], new_vertexs[0], new_vertexs[1]);
				render_list.push_back(new_t3d * GamePipeline.GetProjTrans());
				return;
			}
			else
			{
				Triangle3D new_t3d(in_vertexs[0], new_vertexs[1], new_vertexs[0]);
				render_list.push_back(new_t3d * GamePipeline.GetProjTrans());
				return;
			}
		}
		else if (out_vertexs.size() == 1)
		{
			if (new_vertexs[0].pos.x < new_vertexs[1].pos.x)
			{
				if (in_vertexs[0].pos.x < in_vertexs[1].pos.x)
				{
					Triangle3D new_t3d0(in_vertexs[0], new_vertexs[0], new_vertexs[1]);
					Triangle3D new_t3d1(in_vertexs[0], new_vertexs[1], in_vertexs[1]);
					render_list.push_back(new_t3d0 * GamePipeline.GetProjTrans());
					render_list.push_back(new_t3d1 * GamePipeline.GetProjTrans());
					return;
				}
				else
				{
					Triangle3D new_t3d0(in_vertexs[1], new_vertexs[0], new_vertexs[1]);
					Triangle3D new_t3d1(in_vertexs[1], new_vertexs[1], in_vertexs[0]);
					render_list.push_back(new_t3d0 * GamePipeline.GetProjTrans());
					render_list.push_back(new_t3d1 * GamePipeline.GetProjTrans());
					return;
				}
			}
			else
			{
				if (in_vertexs[0].pos.x < in_vertexs[1].pos.x)
				{
					Triangle3D new_t3d0(in_vertexs[0], new_vertexs[1], new_vertexs[0]);
					Triangle3D new_t3d1(in_vertexs[0], new_vertexs[0], in_vertexs[1]);
					render_list.push_back(new_t3d0 * GamePipeline.GetProjTrans());
					render_list.push_back(new_t3d1 * GamePipeline.GetProjTrans());
					return;
				}
				else
				{
					Triangle3D new_t3d0(in_vertexs[1], new_vertexs[1], new_vertexs[0]);
					Triangle3D new_t3d1(in_vertexs[1], new_vertexs[0], in_vertexs[0]);
					render_list.push_back(new_t3d0 * GamePipeline.GetProjTrans());
					render_list.push_back(new_t3d1 * GamePipeline.GetProjTrans());
					return;
				}
			}
		}
		return;
	}
	else if (v0.z > 1 && v1.z > 1 && v2.z > 1)
	{
		return;
	}
	
	render_list.push_back(spaceTriangle3D);
}

void RenderList::Render()
{
	for (int i = 0; i < render_list.size(); i++)
	{
		render_list[i].Show();
	}
}

//void RenderList::AddTriangle(const Triangle3D &t3d)
//{
//	bool out[3];
//	int out_count = 0;
//	for (int i = 0; i < 3; i++)
//	{
//		//printf("t3d (%f, %f, %f)\n", t3d.m_vs[i].pos.x, t3d.m_vs[i].pos.y, t3d.m_vs[i].pos.z);
//		Vector3f vec3 = t3d.m_vs[i].pos.To3D();
//		Vector3f v0 = vec3 - Vector3f(-1, 1, 1);
//		Vector3f v1 = vec3 - Vector3f(1, -1, -1);
//		v0.Normalize();
//		v1.Normalize();
//		float dot = 0.0f;
//		//近
//		dot = -1 * v0.z;
//		if (dot < 0.0f)
//		{
//			out[i] = true;
//			out_count++;
//			continue;
//		}
//		//上
//		dot = -1 * v0.y;
//		if (dot < 0.0f)
//		{
//			out[i] = true;
//			continue;
//		}
//		//下
//		dot = v1.y;
//		if (dot < 0.0f)
//		{
//			out[i] = true;
//			continue;
//		}
//		//左
//		dot = v0.x;
//		if (dot < 0.0f)
//		{
//			out[i] = true;
//			continue;
//		}
//		//右
//		dot = -1 * v1.x;
//		if (dot < 0.0f)
//		{
//			out[i] = true;
//			continue;
//		}
//		//远
//		dot = v1.z;
//		if (dot < 0.0f)
//		{
//			out[i] = true;
//			continue;
//		}
//		////近
//		//dot = -1 * v0.z;
//		//if (dot < 0.0f)
//		//{
//		//	out[i] = true;
//		//	out_count++;
//		//	continue;
//		//}
//	}
//	if (out[0] == true && out[1] == true && out[2] == true)
//	{
//		return;
//	}
//	else if (out_count == 2)
//	{
//		printf("==2");
//		return;
//		if (out[0] == false)
//		{
//			float t = 0.0f;
//			float dx, dy, dr, dg, db;
//			float x1, x2, y1, y2;
//			float r1, g1, b1, r2, g2, b2;
//
//			float dz = t3d.m_vs[1].pos.z - t3d.m_vs[0].pos.z;
//			t = (ZNEAR - t3d.m_vs[0].pos.z) / dz;
//
//			dx = t3d.m_vs[1].pos.x - t3d.m_vs[0].pos.x;
//			dy = t3d.m_vs[1].pos.y - t3d.m_vs[0].pos.y;
//			dr = t3d.m_vs[1].color.x - t3d.m_vs[0].color.x;
//			dg = t3d.m_vs[1].color.y - t3d.m_vs[0].color.y;
//			db = t3d.m_vs[1].color.z - t3d.m_vs[0].color.z;
//			x1 = t3d.m_vs[0].pos.x + t * dx;
//			y1 = t3d.m_vs[0].pos.y + t * dy;
//			r1 = t3d.m_vs[0].color.x + t * dr;
//			g1 = t3d.m_vs[0].color.y + t * dg;
//			b1 = t3d.m_vs[0].color.z + t * db;
//
//			dz = t3d.m_vs[2].pos.z - t3d.m_vs[0].pos.z;
//			t = (ZNEAR - t3d.m_vs[0].pos.z) / dz;
//			dx = t3d.m_vs[2].pos.x - t3d.m_vs[0].pos.x;
//			dy = t3d.m_vs[2].pos.y - t3d.m_vs[0].pos.y;
//			dr = t3d.m_vs[2].color.x - t3d.m_vs[0].color.x;
//			dg = t3d.m_vs[2].color.y - t3d.m_vs[0].color.y;
//			db = t3d.m_vs[2].color.z - t3d.m_vs[0].color.z;
//			x2 = t3d.m_vs[0].pos.x + t * dx;
//			y2 = t3d.m_vs[0].pos.y + t * dy;
//			r2 = t3d.m_vs[0].color.x + t * dr;
//			g2 = t3d.m_vs[0].color.y + t * dg;
//			b2 = t3d.m_vs[0].color.z + t * db;
//		}
//	}
//	else if (out_count == 1)
//	{
//		printf("==1\n");
//		if (out[0] == true)
//		{
//			float t = 0.0f;
//			float dx, dy, dr, dg, db;
//			float x1, x2, y1, y2;
//			float r1, g1, b1, r2, g2, b2;
//
//			float dz = t3d.m_vs[1].pos.z - t3d.m_vs[0].pos.z;
//			t = (ZNEAR - t3d.m_vs[0].pos.z) / dz;
//			dx = t3d.m_vs[1].pos.x - t3d.m_vs[0].pos.x;
//			dy = t3d.m_vs[1].pos.y - t3d.m_vs[0].pos.y;
//			dr = t3d.m_vs[1].color.x - t3d.m_vs[0].color.x;
//			dg = t3d.m_vs[1].color.y - t3d.m_vs[0].color.y;
//			db = t3d.m_vs[1].color.z - t3d.m_vs[0].color.z;
//			x1 = t3d.m_vs[0].pos.x + t * dx;
//			y1 = t3d.m_vs[0].pos.y + t * dy;
//			r1 = t3d.m_vs[0].color.x + t * dr;
//			g1 = t3d.m_vs[0].color.y + t * dg;
//			b1 = t3d.m_vs[0].color.z + t * db;
//
//			dz = t3d.m_vs[2].pos.z - t3d.m_vs[0].pos.z;
//			t = (ZNEAR - t3d.m_vs[0].pos.z) / dz;
//			dx = t3d.m_vs[2].pos.x - t3d.m_vs[0].pos.x;
//			dy = t3d.m_vs[2].pos.y - t3d.m_vs[0].pos.y;
//			dr = t3d.m_vs[2].color.x - t3d.m_vs[0].color.x;
//			dg = t3d.m_vs[2].color.y - t3d.m_vs[0].color.y;
//			db = t3d.m_vs[2].color.z - t3d.m_vs[0].color.z;
//			x2 = t3d.m_vs[0].pos.x + t * dx;
//			y2 = t3d.m_vs[0].pos.y + t * dy;
//			r2 = t3d.m_vs[0].color.x + t * dr;
//			g2 = t3d.m_vs[0].color.y + t * dg;
//			b2 = t3d.m_vs[0].color.z + t * db;
//
//			Vertex right_vertex(Vector3f(x1, y1, ZNEAR), Vector3f(r1, g1, b1)); //靠右
//			Vertex left_vertex(Vector3f(x2, y2, ZNEAR), Vector3f(r2, g2, b2)); //靠左
//
//			Triangle3D t1(t3d.m_vs[1], t3d.m_vs[2], left_vertex);
//			Triangle3D t2(t3d.m_vs[1], left_vertex, right_vertex);
//			t1.face_render = t3d.face_render;
//			t2.face_render = t3d.face_render;
//			render_list.push_back(t1);
//			render_list.push_back(t2);
//			return;
//		}
//		else if (out[1] == true)
//		{
//			float t = 0.0f;
//			float dx, dy, dr, dg, db;
//			float x1, x2, y1, y2;
//			float r1, g1, b1, r2, g2, b2;
//
//			float dz = t3d.m_vs[0].pos.z - t3d.m_vs[1].pos.z;
//			t = (ZNEAR - t3d.m_vs[1].pos.z) / dz;
//			//left
//			dx = t3d.m_vs[0].pos.x - t3d.m_vs[1].pos.x;
//			dy = t3d.m_vs[0].pos.y - t3d.m_vs[1].pos.y;
//			dr = t3d.m_vs[0].color.x - t3d.m_vs[1].color.x;
//			dg = t3d.m_vs[0].color.y - t3d.m_vs[1].color.y;
//			db = t3d.m_vs[0].color.z - t3d.m_vs[1].color.z;
//			x1 = t3d.m_vs[1].pos.x + t * dx;
//			y1 = t3d.m_vs[1].pos.y + t * dy;
//			r1 = t3d.m_vs[1].color.x + t * dr;
//			g1 = t3d.m_vs[1].color.y + t * dg;
//			b1 = t3d.m_vs[1].color.z + t * db;
//
//			//right
//			dz = t3d.m_vs[2].pos.z - t3d.m_vs[1].pos.z;
//			t = (ZNEAR - t3d.m_vs[0].pos.z) / dz;
//			dx = t3d.m_vs[2].pos.x - t3d.m_vs[1].pos.x;
//			dy = t3d.m_vs[2].pos.y - t3d.m_vs[1].pos.y;
//			dr = t3d.m_vs[2].color.x - t3d.m_vs[1].color.x;
//			dg = t3d.m_vs[2].color.y - t3d.m_vs[1].color.y;
//			db = t3d.m_vs[2].color.z - t3d.m_vs[1].color.z;
//			x2 = t3d.m_vs[1].pos.x + t * dx;
//			y2 = t3d.m_vs[1].pos.y + t * dy;
//			r2 = t3d.m_vs[1].color.x + t * dr;
//			g2 = t3d.m_vs[1].color.y + t * dg;
//			b2 = t3d.m_vs[1].color.z + t * db;
//
//			Vertex right_vertex(Vector3f(x2, y2, ZNEAR), Vector3f(r2, g2, b2)); //靠右
//			Vertex left_vertex(Vector3f(x1, y1, ZNEAR), Vector3f(r1, g1, b1)); //靠左
//
//			Triangle3D t1(t3d.m_vs[2], t3d.m_vs[0], left_vertex);
//			Triangle3D t2(t3d.m_vs[2], left_vertex, right_vertex);
//			t1.face_render = t3d.face_render;
//			t2.face_render = t3d.face_render;
//			render_list.push_back(t1);
//			render_list.push_back(t2);
//			return;
//		}
//		else //2
//		{
//			float t = 0.0f;
//			float dx, dy, dr, dg, db;
//			float x1, x2, y1, y2;
//			float r1, g1, b1, r2, g2, b2;
//
//			//left
//			float dz = t3d.m_vs[1].pos.z - t3d.m_vs[2].pos.z;
//			t = (ZNEAR - t3d.m_vs[2].pos.z) / dz;
//			dx = t3d.m_vs[1].pos.x - t3d.m_vs[2].pos.x;
//			dy = t3d.m_vs[1].pos.y - t3d.m_vs[2].pos.y;
//			dr = t3d.m_vs[1].color.x - t3d.m_vs[2].color.x;
//			dg = t3d.m_vs[1].color.y - t3d.m_vs[2].color.y;
//			db = t3d.m_vs[1].color.z - t3d.m_vs[2].color.z;
//			x1 = t3d.m_vs[2].pos.x + t * dx;
//			y1 = t3d.m_vs[2].pos.y + t * dy;
//			r1 = t3d.m_vs[2].color.x + t * dr;
//			g1 = t3d.m_vs[2].color.y + t * dg;
//			b1 = t3d.m_vs[2].color.z + t * db;
//
//			//right
//			dz = t3d.m_vs[0].pos.z - t3d.m_vs[2].pos.z;
//			t = (ZNEAR - t3d.m_vs[0].pos.z) / dz;
//			dx = t3d.m_vs[0].pos.x - t3d.m_vs[2].pos.x;
//			dy = t3d.m_vs[0].pos.y - t3d.m_vs[2].pos.y;
//			dr = t3d.m_vs[0].color.x - t3d.m_vs[2].color.x;
//			dg = t3d.m_vs[0].color.y - t3d.m_vs[2].color.y;
//			db = t3d.m_vs[0].color.z - t3d.m_vs[2].color.z;
//			x2 = t3d.m_vs[2].pos.x + t * dx;
//			y2 = t3d.m_vs[2].pos.y + t * dy;
//			r2 = t3d.m_vs[2].color.x + t * dr;
//			g2 = t3d.m_vs[2].color.y + t * dg;
//			b2 = t3d.m_vs[2].color.z + t * db;
//
//			Vertex right_vertex(Vector3f(x1, y1, ZNEAR), Vector3f(r1, g1, b1)); //靠右
//			Vertex left_vertex(Vector3f(x2, y2, ZNEAR), Vector3f(r2, g2, b2)); //靠左
//
//			Triangle3D t1(t3d.m_vs[0], t3d.m_vs[1], left_vertex);
//			Triangle3D t2(t3d.m_vs[0], left_vertex, right_vertex);
//			t1.face_render = t3d.face_render;
//			t2.face_render = t3d.face_render;
//			render_list.push_back(t1);
//			render_list.push_back(t2);
//			return;
//		}
//	}
//	render_list.push_back(t3d);
//}