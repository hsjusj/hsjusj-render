#include "math_3d.h"

Matrix4f::Matrix4f(float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33)
{
	m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
	m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
	m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
	m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
}

Matrix4f::Matrix4f(const Matrix4f &m4f)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = m4f.m[i][j];
		}
	}
}

Matrix4f &Matrix4f::operator=(const Matrix4f &m4f)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = m4f.m[i][j];
		}
	}
	return *this;
}

Matrix4f Matrix4f::operator*(const Matrix4f &m4f)
{
	Matrix4f new_m4f;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			new_m4f.m[i][j] = m[i][0] * m4f.m[0][j] +
							  m[i][1] * m4f.m[1][j] +
							  m[i][2] * m4f.m[2][j] +
							  m[i][3] * m4f.m[3][j];
		}
	}
	return new_m4f;
}

Matrix4f &Matrix4f::operator*=(const Matrix4f &m4f)
{
	Matrix4f new_m4f;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			new_m4f.m[i][j] = m[i][0] * m4f.m[0][j] +
				m[i][1] * m4f.m[1][j] +
				m[i][2] * m4f.m[2][j] +
				m[i][3] * m4f.m[3][j];
		}
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

void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	m[0][0] = ScaleX; m[0][1] = 0.0f,   m[0][2] = 0.0f;   m[0][3] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
	m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleY; m[2][3] = 0.0f;
	m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	Matrix4f rx, ry, rz;

	const float x = ToRadian(RotateX);
	const float y = ToRadian(RotateY);
	const float z = ToRadian(RotateZ);

	rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f;    rx.m[0][2] = 0.0f;     rx.m[0][3] = 0.0f;
	rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
	rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x);  rx.m[2][3] = 0.0f;
	rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f;    rx.m[3][2] = 0.0f;     rx.m[3][3] = 1.0f;

	ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
	ry.m[1][0] = 0.0f;    ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f;     ry.m[1][3] = 0.0f;
	ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y);  ry.m[2][3] = 0.0f;
	ry.m[3][0] = 0.0f;    ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f;     ry.m[3][3] = 1.0f;

	rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
	rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z);  rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
	rz.m[2][0] = 0.0f;    rz.m[2][1] = 0.0f;     rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
	rz.m[3][0] = 0.0f;    rz.m[3][1] = 0.0f;     rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

	*this = rz * ry * rx;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4f::InitPersProjTransform(const PersProjInfo &p)
{
	const float ar = p.Width / p.Height;
	const float zRange = p.zNear - p.zFar;
	const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));

    m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;            m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f/tanHalfFOV; m[1][2] = 0.0f;            m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-p.zNear - p.zFar)/zRange ; m[2][3] = 2.0f*p.zFar*p.zNear/zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;            m[3][3] = 0.0;   
}