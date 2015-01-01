//====================================================================================================
//	Filename:		Camera.h
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================
#include "precompiled.h"
#include "Camera.h"

//====================================================================================================
//namespace
//====================================================================================================
namespace MAGE3D
{
namespace Graphics
{

//====================================================================================================
//class definitons
//====================================================================================================


Camera::Camera()
	:mPosition(0.0f, 0.0f, 0.0f)
	,mRight(1.0f, 0.0f, 0.0f)
	,mUp(0.0f, 1.0f, 0.0f)
	,mLook(0.0f, 0.0f, 1.0f)
	,mFOV(Math::kPi * 0.25f)
	,mAspectRatio(4.0f / 3.0f)
	,mNearPlane(1.0f)
	,mFarPlane(10000.0f)
{

}

//----------------------------------------------------------------------------------------------------

Camera::~Camera()
{

}

//----------------------------------------------------------------------------------------------------

void Camera::Strafe(float distance)
{
	mPosition += (mRight * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Rise(float distance)
{
	mPosition += (mUp * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Walk(float distance)
{
	mPosition += (mLook * distance);
}

//----------------------------------------------------------------------------------------------------


void Camera::Pitch(float angle)
{
	// rotate look and up around the right vector
	Math::Matrix4 matRotate;
	matRotate.SetRotateAxis(mRight, angle);
	mUp = Math::TransformNormal(matRotate, mUp);
	mLook = Math::TransformNormal(matRotate, mLook);
	
	// Renormalize the basis vectors
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::Yaw(float angle)
{
	// rotate look and right around the Y-axis vector
	Math::Matrix4 matRotate;
	matRotate.SetRotateAxis(Math::Vector3::YAxis, angle);
	mRight = Math::TransformNormal(matRotate, mRight);
	mLook = Math::TransformNormal(matRotate, mLook);
	
	// Renormalize the basis vectors
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::Roll(float angle)
{
	// rotate right and up around the look vector
	Math::Matrix4 matRotate;
	matRotate.SetRotateAxis(mLook, angle);
	mRight = Math::TransformNormal(matRotate, mRight);
	mUp = Math::TransformNormal(matRotate, mUp);
	
	
	// Renormalize the basis vectors
	Renormalize();
}

//----------------------------------------------------------------------------------------------------


void Camera::PitchAroundLook(float angle)
{
	// rotate look and up around the right vector
	Math::Matrix4 matRotate;
	matRotate.SetRotateAxis(mRight, angle);
	mUp = Math::TransformNormal(matRotate, mUp);
	mLook = Math::TransformNormal(matRotate, mLook);
	
	// Renormalize the basis vectors
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::YawAround(Math::Vector3 target, float angle)
{
	// rotate look and right around the Y-axis vector
	Math::Matrix4 matRotate;
	matRotate.SetRotateAxis(target, angle);
	mRight = Math::TransformNormal(matRotate, mRight);
	mLook = Math::TransformNormal(matRotate, mLook);
	
	// Renormalize the basis vectors
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::RollAroundLook(float angle)
{
	// rotate right and up around the look vector
	Math::Matrix4 matRotate;
	matRotate.SetRotateAxis(mLook, angle);
	mRight = Math::TransformNormal(matRotate, mRight);
	mUp = Math::TransformNormal(matRotate, mUp);
	
	
	// Renormalize the basis vectors
	Renormalize();
}

//----------------------------------------------------------------------------------------------------


const Math::Vector3& Camera::GetPosition() const
{
	return mPosition;
}

//----------------------------------------------------------------------------------------------------

const Math::Vector3& Camera::GetDirection() const
{
	return mLook;
}

//----------------------------------------------------------------------------------------------------

Math::Matrix4 Camera::GetViewMatrix() const
{
	// calculate the translation compoonents 
	const float dx = -Math::Dot(mPosition, mRight);
	const float dy = -Math::Dot(mPosition, mUp);
	const float dz = -Math::Dot(mPosition, mLook);
	
	// create view matrix
	Math::Matrix4 matView;
	matView._11 = mRight.x;
	matView._21 = mRight.y;
	matView._31 = mRight.z;
	matView._41 = dx;

	matView._12 = mUp.x;
	matView._22 = mUp.y;
	matView._32 = mUp.z;
	matView._42 = dy;
	
	matView._13 = mLook.x;
	matView._23 = mLook.y;
	matView._33 = mLook.z;
	matView._43 = dz;

	matView._14 = 0.0f;
	matView._24 = 0.0f;
	matView._34 = 0.0f;
	matView._44 = 1.0f;

	return matView;
}

//----------------------------------------------------------------------------------------------------

Math::Matrix4 Camera::GetProjectionMatrix() const
{
	const float w = 1.0f / tan(mFOV * 0.5f * mAspectRatio);
	const float h = 1.0f / tan(mFOV * 0.5f);
	const float q = mFarPlane / (mFarPlane - mNearPlane);

	// create the projection matrix
	Math::Matrix4 matProj;
	matProj._11 = w;
	matProj._21 = 0.0f;
	matProj._31 = 0.0f;
	matProj._41 = 0.0f;

	matProj._12 = 0.0f;
	matProj._22 = h;
	matProj._32 = 0.0f;
	matProj._42 = 0.0f;

	matProj._13 = 0.0f;
	matProj._23 = 0.0f;
	matProj._33 = q;
	matProj._43 = -q * mNearPlane;

	matProj._14 = 0.0f;
	matProj._24 = 0.0f;
	matProj._34 = 1.0f;
	matProj._44 = 0.0f;
	return matProj;
}

//----------------------------------------------------------------------------------------------------


void Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetDirection(const Math::Vector3& direction)
{
	mLook = direction;



	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::SetFOV(float fov)
{
	mFOV = fov;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetNearPlane(float distance)
{
	mNearPlane = distance;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetFarPlane(float distance)
{
	mFarPlane = distance;
}

//----------------------------------------------------------------------------------------------------

void Camera::Renormalize()
{
	mLook.Normalize();
	mUp = mLook.CrossProduct(mRight);
	mUp.Normalize();
	mRight = mUp.CrossProduct(mLook);
	mRight.Normalize();

}



}//namespace MAGE3D
}//namespace Graphics



