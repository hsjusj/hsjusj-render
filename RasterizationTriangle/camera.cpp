#include "camera.h"

const static float STEP_SCALE = 0.1f;

Camera::Camera() : m_pos(Vector3f(0.0f, 0.0f, 0.0f)), m_target(Vector3f(0.0f, 0.0f, 1.0f)), m_up(Vector3f(0.0f, 1.0f, 0.0f))
{

}

Camera::Camera(const Vector3f &Pos, const Vector3f &Target, const Vector3f &Up) : m_pos(Pos), m_target(Target), m_up(Up)
{

}

void Camera::OnKeyboard(int key)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			m_pos += (m_target * STEP_SCALE);
		}
		break;
	case GLUT_KEY_DOWN:
		{
			m_pos -= (m_target * STEP_SCALE);
		}
		break;
	case GLUT_KEY_LEFT:
		{
			Vector3f U = m_up.Cross(m_target);
			U.Normalize();
			m_pos -= (U * STEP_SCALE);
		}
		break;
	case GLUT_KEY_RIGHT:
		{
		   Vector3f U = m_up.Cross(m_target);
		   U.Normalize();
		   m_pos += (U * STEP_SCALE);
		}
		break;
	default:
		break;
	}
	return;
}