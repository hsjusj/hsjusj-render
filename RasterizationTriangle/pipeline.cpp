#include "pipeline.h"

Pipeline::Pipeline() : m_scale(Vector3f(1.0f, 1.0f, 1.0f)), m_rotateInfo(Vector3f(0.0f, 0.0f, 0.0f)), m_worldPos(Vector3f(0.0f, 0.0f, 0.0f))
{

}

void Pipeline::Scale(float s)
{
	m_scale = Vector3f(s, s, s);
}

void Pipeline::Scale(const Vector3f &s)
{
	m_scale = s;
}

void Pipeline::Scale(float sx, float sy, float sz)
{
	m_scale = Vector3f(sx, sy, sz);
}

void Pipeline::WorldPos(float x, float y, float z)
{
	m_worldPos = Vector3f(x, y, z);
}

void Pipeline::WorldPos(const Vector3f &w)
{
	m_worldPos = w;
}

void Pipeline::Rotate(float rx, float ry, float rz)
{
	m_rotateInfo = Vector3f(rx, ry, rz);
}

void Pipeline::Rotate(const Vector3f &r)
{
	m_rotateInfo = r;
}

void Pipeline::SetCamera(const Vector3f &Pos, const Vector3f &Target, const Vector3f &Up)
{
	m_camera.Pos = Pos;
	m_camera.Target = Target;
	m_camera.Up = Up;
}

void Pipeline::SetCamera(const Camera &camera)
{
	m_camera.Pos = camera.GetPos();
	m_camera.Target = camera.GetTarget();
	m_camera.Up = camera.GetUp();
}

void Pipeline::SetPerspectiveProj(const PersProjInfo &p)
{
	m_persProjInfo = p;
	m_ProjTransformation.InitPersProjTransform(m_persProjInfo);
}

const Matrix4f &Pipeline::GetWPTrans()
{
	GetWorldTrans();
	m_WPtransformation = m_ProjTransformation * m_Wtransformation;
	return m_WPtransformation;
}

const Matrix4f &Pipeline::GetWVTrans()
{
	GetWorldTrans();
	GetViewTrans();
	m_WVtransformation = m_Vtransforamtion * m_Wtransformation;
	return m_WVtransformation;
}

const Matrix4f &Pipeline::GetVPTrans()
{
	GetViewTrans();
	m_VPtransformation = m_ProjTransformation * m_Vtransforamtion;
	return m_VPtransformation;
}

const Matrix4f &Pipeline::GetWVPTrans()
{
	GetWorldTrans();
	GetViewTrans();
	m_WVPtransformation = m_ProjTransformation * m_Vtransforamtion * m_Wtransformation;
	return m_WVPtransformation;
}

const Matrix4f &Pipeline::GetWorldTrans()
{
	Matrix4f ScaleMatrix, RotateMatrix, TransMatrix;
	ScaleMatrix.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	RotateMatrix.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	TransMatrix.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

	m_Wtransformation = TransMatrix * RotateMatrix * ScaleMatrix;
	return m_Wtransformation;
}

const Matrix4f &Pipeline::GetViewTrans()
{
	Matrix4f CameraTranslationTrans, CameraRotateTrans;

	CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
	m_Vtransforamtion = CameraRotateTrans * CameraTranslationTrans;
	return m_Vtransforamtion;
}

const Matrix4f &Pipeline::GetProjTrans()
{
	return m_ProjTransformation;
}