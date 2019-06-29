#ifndef MATH3D_H__

#define MATH3D_H__

#include <math.h>

#define M_PI 3.14f
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

class Vector2f
{
public:
	float x, y, z;
	Vector2f(float = 0.0f, float = 0.0f);
	Vector2f(const Vector2f&);

	Vector2f &operator=(const Vector2f&);
	Vector2f operator+(const Vector2f&);
	Vector2f operator-(const Vector2f&);
	Vector2f &operator+=(const Vector2f&);
	Vector2f &operator-=(const Vector2f&);

	bool operator==(const Vector2f&);
	bool operator!=(const Vector2f&);
};

class Vector3f
{
public:
	float x, y, z, w;
	Vector3f(float = 0.0f, float = 0.0f, float = 0.0f, float = 1.0f);
	Vector3f(const Vector3f&);
	
	Vector3f &operator=(const Vector3f&);
	Vector3f operator+(const Vector3f&);
	Vector3f operator-(const Vector3f&);
	Vector3f &operator+=(const Vector3f&);
	Vector3f &operator-=(const Vector3f&);

	bool operator==(const Vector3f&);
	bool operator!=(const Vector3f&);

	Vector3f &Normalize();
	Vector3f To3D()const;
};

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

class Matrix4f
{
public:
	float m[4][4];
	Matrix4f(float = 1.0f, float = 0.0f, float = 0.0f, float = 0.0f,
		float = 0.0f, float = 1.0f, float = 0.0f, float = 0.0f,
		float = 0.0f, float = 0.0f, float = 1.0f, float = 0.0f,
		float = 0.0f, float = 0.0f, float = 0.0f, float = 1.0f);
	Matrix4f(const Matrix4f&);
	Matrix4f &operator=(const Matrix4f&);

	Matrix4f operator*(const Matrix4f&);
	Matrix4f &operator*=(const Matrix4f&);
	Vector3f operator*(const Vector3f&) const;

	void InitScaleTransform(float, float, float);
	void InitRotateTransform(float, float, float);
	void InitTranslationTransform(float, float, float);
	void InitPersProjTransform(const PersProjInfo&);
};

class Vertex
{
public:
	Vector3f pos;
	Vector3f color;
	Vector3f normal;
	Vertex();
	Vertex(const Vertex&);
	Vertex(const Vector3f&);
	Vertex(const Vector3f&, const Vector3f&);

	Vertex& operator=(const Vertex&);
};

#endif