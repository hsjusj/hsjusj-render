#ifndef PIPELINE_H__

#define PIPELINE_H__

#include "math_3d.h"
#include "camera.h"

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

	void SetCamera(const Vector3f &Pos, const Vector3f &Target, const Vector3f &Up);
	void SetCamera(const Camera&);

	void SetPerspectiveProj(const PersProjInfo&);

	const Matrix4f &GetWPTrans();
	const Matrix4f &GetWVTrans();
	const Matrix4f &GetVPTrans();
	const Matrix4f &GetWVPTrans();
	const Matrix4f &GetWorldTrans();
	const Matrix4f &GetViewTrans();
	const Matrix4f &GetProjTrans();
private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	PersProjInfo m_persProjInfo;

	struct
	{
		Vector3f Pos;
		Vector3f Target;
		Vector3f Up;
	} m_camera;

	Matrix4f m_WVPtransformation;
	Matrix4f m_VPtransformation;
	Matrix4f m_WPtransformation;
	Matrix4f m_WVtransformation;
	Matrix4f m_Wtransformation;
	Matrix4f m_Vtransforamtion;
	Matrix4f m_ProjTransformation;
};

#endif