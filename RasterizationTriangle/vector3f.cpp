#include "math_3d.h"
#include <cstdio>

Vector3f::Vector3f(float x0, float y0, float z0, float w0) : x(x0), y(y0), z(z0), w(w0)
{

}

Vector3f::Vector3f(const Vector3f &v3f) : x(v3f.x), y(v3f.y), z(v3f.z), w(v3f.w)
{

}

Vector3f &Vector3f::operator=(const Vector3f &v3f)
{
	x = v3f.x;
	y = v3f.y;
	z = v3f.z;
	w = v3f.w;
	return *this;
}

Vector3f Vector3f::operator+(const Vector3f &v3f)
{
	return Vector3f(x + v3f.x, y + v3f.y, z + v3f.z, 1.0f);
}

Vector3f Vector3f::operator-(const Vector3f &v3f)
{
	return Vector3f(x - v3f.x, y - v3f.y, z - v3f.z, 1.0f);
}

Vector3f &Vector3f::operator+=(const Vector3f &v3f)
{
	x += v3f.x;
	y += v3f.y;
	z += v3f.z;
	w = 1.0f;
	return *this;
}

Vector3f &Vector3f::operator-=(const Vector3f &v3f)
{
	x -= v3f.x;
	y -= v3f.y;
	z -= v3f.z;
	w = 1.0f;
	return *this;
}

float Vector3f::operator*(const Vector3f &v3f) const
{
	return x * v3f.x + y * v3f.y + z * v3f.z;
}

bool Vector3f::operator==(const Vector3f &v3f)
{
	return x == v3f.x && y == v3f.y && z == v3f.z;
}

bool Vector3f::operator!=(const Vector3f &v3f)
{
	return x != v3f.x || y != v3f.y || z != v3f.z;
}

Vector3f &Vector3f::Normalize()
{
	float temp = sqrtf(x * x + y * y + z * z);
	x /= temp;
	y /= temp;
	z /= temp;
	return *this;
}

Vector3f Vector3f::Cross(const Vector3f &v3f) const
{
	Vector3f cross;
	cross.x = y * v3f.z - z * v3f.y;
	cross.y = z * v3f.x - x * v3f.z;
	cross.z = x * v3f.y - y * v3f.x;
	return cross;
}

Vector3f Vector3f::To3D() const
{
	return Vector3f(x / w, y / w, z / w, w/w);
}

void Vector3f::Print() const
{
	printf("(%f, %f, %f, %f)\n", x, y, z, w);
}