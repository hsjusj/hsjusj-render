#ifndef MATH3D_H__

#define MATH3D_H__

#include <math.h>

#define M_PI 3.14f
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

static float Fast_SquareRT(float f);

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

	float operator*(const Vector3f&) const;
	Vector3f operator*(const float) const;
	Vector3f &operator*=(const float);

	bool operator==(const Vector3f&);
	bool operator!=(const Vector3f&);

	Vector3f &Normalize();
	Vector3f Cross(const Vector3f&) const;
	Vector3f To3D() const;

	void Print() const;
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
	Matrix4f(float a00 = 1.0f, float a01 = 0.0f, float a02 = 0.0f, float a03 = 0.0f,
		  	 float a10 = 0.0f, float a11 = 1.0f, float a12 = 0.0f, float a13 = 0.0f,
			 float a20 = 0.0f, float a21 = 0.0f, float a22 = 1.0f, float a23 = 0.0f,
			 float a30 = 0.0f, float a31 = 0.0f, float a32 = 0.0f, float a33 = 1.0f)
	{
		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
		m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
	}

	Matrix4f::Matrix4f(const Matrix4f &m4f)
	{
		m[0][0] = m4f.m[0][0]; m[0][1] = m4f.m[0][1]; m[0][2] = m4f.m[0][2]; m[0][3] = m4f.m[0][3];
		m[1][0] = m4f.m[1][0]; m[1][1] = m4f.m[1][1]; m[1][2] = m4f.m[1][2]; m[1][3] = m4f.m[1][3];
		m[2][0] = m4f.m[2][0]; m[2][1] = m4f.m[2][1]; m[2][2] = m4f.m[2][2]; m[2][3] = m4f.m[2][3];
		m[3][0] = m4f.m[3][0]; m[3][1] = m4f.m[3][1]; m[3][2] = m4f.m[3][2]; m[3][3] = m4f.m[3][3];
	}

	Matrix4f &Matrix4f::operator=(const Matrix4f &m4f)
	{
		m[0][0] = m4f.m[0][0]; m[0][1] = m4f.m[0][1]; m[0][2] = m4f.m[0][2]; m[0][3] = m4f.m[0][3];
		m[1][0] = m4f.m[1][0]; m[1][1] = m4f.m[1][1]; m[1][2] = m4f.m[1][2]; m[1][3] = m4f.m[1][3];
		m[2][0] = m4f.m[2][0]; m[2][1] = m4f.m[2][1]; m[2][2] = m4f.m[2][2]; m[2][3] = m4f.m[2][3];
		m[3][0] = m4f.m[3][0]; m[3][1] = m4f.m[3][1]; m[3][2] = m4f.m[3][2]; m[3][3] = m4f.m[3][3];
		return *this;
	}

	Matrix4f Matrix4f::operator*(const Matrix4f &m4f)
	{
		Matrix4f new_m4f;
		for (int i = 0; i < 4; i++)
		{
			new_m4f.m[i][0] = m[i][0] * m4f.m[0][0] +
						      m[i][1] * m4f.m[1][0] +
							  m[i][2] * m4f.m[2][0] +
							  m[i][3] * m4f.m[3][0];

			new_m4f.m[i][1] = m[i][0] * m4f.m[0][1] +
							  m[i][1] * m4f.m[1][1] +
							  m[i][2] * m4f.m[2][1] +
							  m[i][3] * m4f.m[3][1];

			new_m4f.m[i][2] = m[i][0] * m4f.m[0][2] +
							  m[i][1] * m4f.m[1][2] +
							  m[i][2] * m4f.m[2][2] +
						      m[i][3] * m4f.m[3][2];

			new_m4f.m[i][3] = m[i][0] * m4f.m[0][3] +
							  m[i][1] * m4f.m[1][3] +
							  m[i][2] * m4f.m[2][3] +
							  m[i][3] * m4f.m[3][3];
		}
		return new_m4f;
	}

	Matrix4f &Matrix4f::operator*=(const Matrix4f &m4f)
	{
		Matrix4f new_m4f;
		for (int i = 0; i < 4; i++)
		{
			new_m4f.m[i][0] = m[i][0] * m4f.m[0][0] +
				m[i][1] * m4f.m[1][0] +
				m[i][2] * m4f.m[2][0] +
				m[i][3] * m4f.m[3][0];

			new_m4f.m[i][1] = m[i][0] * m4f.m[0][1] +
				m[i][1] * m4f.m[1][1] +
				m[i][2] * m4f.m[2][1] +
				m[i][3] * m4f.m[3][1];

			new_m4f.m[i][2] = m[i][0] * m4f.m[0][2] +
				m[i][1] * m4f.m[1][2] +
				m[i][2] * m4f.m[2][2] +
				m[i][3] * m4f.m[3][2];

			new_m4f.m[i][3] = m[i][0] * m4f.m[0][3] +
				m[i][1] * m4f.m[1][3] +
				m[i][2] * m4f.m[2][3] +
				m[i][3] * m4f.m[3][3];
		}
		*this = new_m4f;
		return *this;
	}

	Vector3f Matrix4f::operator*(const Vector3f &v3f) const
	{
		Vector3f new_vec;
		new_vec.x = m[0][0] * v3f.x + m[0][1] * v3f.y + m[0][2] * v3f.z + m[0][3] * v3f.w;
		new_vec.y = m[1][0] * v3f.x + m[1][1] * v3f.y + m[1][2] * v3f.z + m[1][3] * v3f.w;
		new_vec.z = m[2][0] * v3f.x + m[2][1] * v3f.y + m[2][2] * v3f.z + m[2][3] * v3f.w;
		new_vec.w = m[3][0] * v3f.x + m[3][1] * v3f.y + m[3][2] * v3f.z + m[3][3] * v3f.w;
		return new_vec;
	}

	void InitScaleTransform(float, float, float);
	void InitRotateTransform(float, float, float);
	void InitTranslationTransform(float, float, float);
	void InitCameraTransform(const Vector3f &Target, const Vector3f &Up);
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