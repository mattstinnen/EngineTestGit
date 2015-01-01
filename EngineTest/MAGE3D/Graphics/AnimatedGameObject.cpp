#include <Precompiled.h>
#include "AnimatedGameObject.h"
#include <vector>




namespace MAGE3D
{
namespace Graphics
{


//====================================================================================================
// class definition
//====================================================================================================
AnimatedGameObject::AnimatedGameObject()
	:mPosition(0.0f,0.0f,0.0f)
	,mDirection(0.0f,0.0f,1.0f)
	,mLook(0.0f,0.0f,1.0f)
	,mScale(1.0f,1.0f,1.0f)
	,mRadius(0.0f)
	,mAnimationIndex(kStill)
{
	mTransform.SetIdentity();
}

AnimatedGameObject::~AnimatedGameObject()
{

}

//----------------------------------------------------------------------------------------------------

void AnimatedGameObject::Load(Graphics::SkinnedMesh* pSkinMesh)
{
	mSkinMesh = pSkinMesh;
	mSkinMesh->SetAnimation(mAnimationIndex);
	mTransform.SetIdentity();
}

//----------------------------------------------------------------------------------------------------

void AnimatedGameObject::Unload()
{
	mSkinMesh = NULL;
}

//----------------------------------------------------------------------------------------------------

void AnimatedGameObject::Update(float fSeconds)
{
	D3DXMatrixRotationY((D3DXMATRIX*)&mTransform,atan2(mDirection.x,mDirection.z)+ D3DX_PI);
	mTransform.SetTranslate(mPosition.x,mPosition.y,mPosition.z);
	mSkinMesh->Update(fSeconds,(D3DXMATRIX*)&mTransform);
	mSkinMesh->SetAnimation(static_cast<int>(mAnimationIndex));
}

//----------------------------------------------------------------------------------------------------

void AnimatedGameObject::Render(IDirect3DDevice9* pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING,false);		// skinned meshes seem to be unlit
	mSkinMesh->Render(pDevice);
	pDevice->SetRenderState(D3DRS_LIGHTING,true);
}

//----------------------------------------------------------------------------------------------------

void AnimatedGameObject::Walk(float distance)
{
	mPosition += (mDirection * distance);
}
void AnimatedGameObject::Strafe(float distance)
{
	Math::Vector3 tempVec = Math::Cross(Math::Vector3::YAxis,mDirection);
	
	mPosition += (tempVec* distance);
}
void AnimatedGameObject::Rise(float distance)
{
	mPosition.y += distance;
}
void AnimatedGameObject::Turn(float angle)
{
	// rotate look and right around the Y-axis vector
	mTransform.SetRotateAxis(Math::Vector3::YAxis, angle);
	mDirection = Math::TransformNormal(mTransform, mDirection);
	mLook.x = mDirection.x;
	mLook.z = mDirection.z;
}
void AnimatedGameObject::Pitch(float angle)
{
	// rotate look and right around the Y-axis vector
	mTransform.SetRotateAxis(Math::Cross(Math::Vector3::YAxis,mDirection), angle);
	mLook = Math::TransformNormal(mTransform, mDirection);
}

}
}