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