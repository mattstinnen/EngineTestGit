#ifndef AnimatedGameObject_H
#define AnimatedGameObject_H

//====================================================================================================
//	Filename:		AnimatedGameObject.h
//	Created by:		Matt Stinnen
//	Discription:	class for a AnimatedGameObject application
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <MAGE3D.h>
#include <Graphics/Sprite.h>

namespace MAGE3D
{
namespace Graphics
{

enum eAnimationType
{
	kDie,
	kAttack,
	kRun,
	kIdle,
	kStill
};
//====================================================================================================
// class declerations
//====================================================================================================

class AnimatedGameObject
{
public:
	AnimatedGameObject();
	~AnimatedGameObject();

	void Load(Graphics::SkinnedMesh* pMesh);
	void Unload();
	void Update(float fSeconds);
	void Render(IDirect3DDevice9* pDevice);

	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);
	void Turn(float angle);
	void Pitch(float angle);


	const Math::Vector3& GetPosition()	const	{return mPosition;}
	const Math::Vector3& GetDirection()	const	{return mDirection;}
	const Math::Vector3& GetScale()		const	{return mScale;}
	const float			 GetRadius()	const	{return mRadius;}

	void SetPosition(Math::Vector3 pos)		{ mPosition = pos;}
	void SetDirection(Math::Vector3 dir)	{ mDirection = dir; mDirection.Normalize();}
	void SetScale(Math::Vector3 scale)		{mScale = scale;}
	void SetRadius(float r)					{mRadius = r;}
	void SetAnimation(eAnimationType index)	{mAnimationIndex = index;}
	void ResetTime()						{mSkinMesh->ResetTime(); }
	
	
protected:
	Math::Matrix4 mTransform;
	Math::Vector3 mPosition;
	Math::Vector3 mDirection;
	Math::Vector3 mLook;
	Math::Vector3 mScale;
	float mRadius;
	Graphics::SkinnedMesh* mSkinMesh;
	eAnimationType mAnimationIndex;

	
	
};
}
}
#endif // #ifndef AnimatedGameObject_H