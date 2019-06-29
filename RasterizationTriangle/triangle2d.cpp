#include "primitive.h"
#include <cstdio>
Triangle2D::Triangle2D(const Vector2f &temp0, const Vector2f &temp1, const Vector2f &temp2) : v0(temp0), v1(temp1), v2(temp2)
{

}

void Triangle2D::Show()
{
	DDA_2d(v0, v1);
	DDA_2d(v1, v2);
	DDA_2d(v2, v0);

	Raster();
}

void Triangle2D::Raster()
{
	Vector2f temp_v0, temp_v1, temp_v2;

	if (v0.y != v1.y && v1.y != v2.y && v0.y != v2.y)
	{
		//Èý½ÇÐÎ·Ö¸î
		if (v0.y > v1.y)
		{
			temp_v0 = v0;
			if (v0.y > v2.y)
			{
				if (v2.y > v1.y)
				{
					temp_v1 = v2;
					temp_v2 = v1;
				}
				else
				{
					temp_v1 = v1;
					temp_v2 = v2;
				}
			}
			else
			{
				temp_v0 = v2;
				temp_v1 = v0;
				temp_v2 = v1;
			}
		}
		else
		{
			temp_v0 = v1;
			if (v1.y > v2.y)
			{
				if (v0.y > v2.y)
				{
					temp_v1 = v0;
					temp_v2 = v2;
				}
				else
				{
					temp_v1 = v2;
					temp_v2 = v0;
				}
			}
			else
			{
				temp_v0 = v2;
				temp_v1 = v1;
				temp_v2 = v0;
			}
		}

		float x = 0.0f, y = temp_v1.y;
		if (temp_v0.x == temp_v2.x)
		{
			x = temp_v0.x;
			y = temp_v1.y;
		}
		else
		{
			float k = (temp_v0.y - temp_v2.y) / (temp_v0.x - temp_v2.x);
			x = temp_v2.x + (y - temp_v2.y) / k;
		}
		Vector2f new_point(x, y);
		Triangle2D t0(temp_v0, temp_v1, new_point);
		Triangle2D t1(temp_v2, temp_v1, new_point);
		t0.Show();
		t1.Show();
		return;
	}

	if (v0.y == v1.y)
	{
		temp_v0 = v2;
		temp_v1 = v1;
		temp_v2 = v0;
	}
	else if (v0.y == v2.y)
	{
		temp_v0 = v1;
		temp_v1 = v0;
		temp_v2 = v2;
	}
	else
	{
		temp_v0 = v0;
		temp_v1 = v1;
		temp_v2 = v2;
	}

	Vector2f v0(temp_v0), v1(temp_v1), v2(temp_v2);

	if (v1.x > v2.x)
	{
		Vector2f temp(v1);
		v1 = v2;
		v2 = temp;
	}

	Vector2f vs(v0);
	Vector2f ve(v0);

	float dx_left = v1.x - v0.x;
	float dx_right = v2.x - v0.x;
	float dy = abs(v0.y - v1.y);

#ifndef CHANGE_DENSITY
	dx_left /= (dy * POINT_DENSITY);
	dx_right /= (dy * POINT_DENSITY);
	dy = v0.y > v1.y ? 1.0f / POINT_DENSITY : -1.0f / POINT_DENSITY;
	float distance = abs(v1.y - v0.y) * POINT_DENSITY;
#else
	dx_left /= (dy * Point_Density);
	dx_right /= (dy * Point_Density);
	dy = v0.y > v1.y ? 1.0f / Point_Density : -1.0f / Point_Density;
	float distance = abs(v1.y - v0.y) * Point_Density;
#endif
	for (int i = 0; i < distance; i++)
	{
		DDA_2d(vs, ve);
		vs.x += dx_left;
		ve.x += dx_right;

		vs.y -= dy;
		ve.y -= dy;
	}
}