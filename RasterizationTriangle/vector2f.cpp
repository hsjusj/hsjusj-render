#include "math_3d.h"

Vector2f::Vector2f(float x0, float y0) : x(x0), y(y0)
{

}

Vector2f::Vector2f(const Vector2f &v2f) : x(v2f.x), y(v2f.y)
{

}

Vector2f &Vector2f::operator=(const Vector2f &v2f)
{
	x = v2f.x;
	y = v2f.y;
	return *this;
}

Vector2f Vector2f::operator+(const Vector2f &v2f)
{
	return Vector2f(x + v2f.x, y + v2f.y);
}

Vector2f Vector2f::operator-(const Vector2f &v2f)
{
	return Vector2f(x - v2f.x, y - v2f.y);
}

Vector2f &Vector2f::operator+=(const Vector2f &v2f)
{
	x += v2f.x;
	y += v2f.y;
	return *this;
}

Vector2f &Vector2f::operator-=(const Vector2f &v2f)
{
	x -= v2f.x;
	y -= v2f.y;
	return *this;
}

bool Vector2f::operator==(const Vector2f &v2f)
{
	return x == v2f.x && y == v2f.y;
}

bool Vector2f::operator!=(const Vector2f &v2f)
{
	return x != v2f.x || y != v2f.y;
}

