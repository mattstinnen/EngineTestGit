#ifndef CAMERA_H
#define CAMERA_H

//====================================================================================================
//	Filename:		Camera.h
//	Created by:		Matt Stinnen
//	Discription:	a class for a 3Dcamera with 6 degrees of freedom
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================

#include "Math/Math.h"

//====================================================================================================
//namespace
//====================================================================================================
namespace MAGE3D
{
namespace Graphics
{

//====================================================================================================
//class declarations
//====================================================================================================

class Camera 
{
public:

	Camera();
	~Camera();

	// functions for camera translations 
	void Strafe(float distance);
	void Rise(float distance);
	void Walk(float distance);

	// functions for camera rotations 
	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);
	void PitchAroundLook(float angle);
	void YawAround(Math::Vector3 target, float angle);
	void RollAroundLook(float angle);

	// accessors
	const Math::Vector3& GetPosition() const;
	const Math::Vector3& GetDirection() const;
	Math::Matrix4 GetViewMatrix() const;
	Math::Matrix4 GetProjectionMatrix() const;

	// mutators
	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);
	void SetFOV(float fov);
	void SetAspectRatio(float ratio);
	void SetNearPlane(float distance);
	void SetFarPlane(float distance);


protected:

	// helper function
	void Renormalize();

	// members
	Math::Vector3 mPosition;
	Math::Vector3 mRight;
	Math::Vector3 mUp;
	Math::Vector3 mLook;
	
	float mFOV;
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;

	
};

}//namespace MAGE3D
}//namespace Graphics



#endif //Camera_H