#ifndef PIPELINE_H__

#define PIPELINE_H__

#include "math_3d.h"

class Pipeline
{
public:
	Pipeline();

	void Scale(float);
	void Scale(const Vector3f&);
	void Scale(float, float, float);

	void WorldPos(float, float, float);
	void WorldPos(const Vector3f&);
	
	void Rotate(float, float, float);
	void Rotate(const Vector3f&);
	
	void SetPerspectiveProj(const PersProjInfo&);

	const Matrix4f &GetWPTrans();
	const Matrix4f &GetWorldTrans();
	const Matrix4f &GetProjTrans();
private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	PersProjInfo m_persProjInfo;

	Matrix4f m_WPtransformation;
	Matrix4f m_Wtransformation;
	Matrix4f m_ProjTransformation;
};

#endif