#ifndef CAMERA_H__

#define CAMERA_H__

#include <gl\freeglut.h>

#include <cstdio>

#include "math_3d.h"

#include "main.h"

class Camera
{
public:
	Camera();
	Camera(const Vector3f&, const Vector3f&, const Vector3f&);
	void OnKeyboard(int key);
	const Vector3f &GetPos() const
	{
		return m_pos;
	}
	const Vector3f &GetTarget() const
	{
		return m_target;
	}
	const Vector3f &GetUp() const
	{
		return m_up;
	}
private:
	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;
};

#endif