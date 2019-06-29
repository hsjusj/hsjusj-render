#include "pipeline.h"

Pipeline::Pipeline()
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

void Pipeline::SetPerspectiveProj(const PersProjInfo &p)
{
	m_persProjInfo = p;
	m_Wtransformation.InitPersProjTransform(m_persProjInfo);
}

const Matrix4f &Pipeline::GetProjTrans()
{
	return m_ProjTransformation;
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

const Matrix4f &Pipeline::GetWPTrans()
{
	GetWorldTrans();
	m_WPtransformation = m_ProjTransformation * m_Wtransformation;
	return m_WPtransformation;
}