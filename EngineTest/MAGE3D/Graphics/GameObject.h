#ifndef GameObject_H
#define GameObject_H

//====================================================================================================
//	Filename:		GameObject.h
//	Created by:		Matt Stinnen
//	Discription:	class for a GameObject application
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


//====================================================================================================
// class declerations
//====================================================================================================

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Load(Graphics::StaticMeshX* pMesh);
	void Unload();
	void Update(float fSeconds);
	void Render(IDirect3DDevice9* pDevice);

	void Walk(float distance);


	const Math::Vector3& GetPosition()	const	{return mPosition;}
	const Math::Vector3& GetDirection()	const	{return mDirection;}
	const Math::Vector3& GetScale()		const	{return mScale;}
	const float			 GetRadius()	const	{return mRadius;}
	bool GetActive()					const	{return IsActive;}

	void SetPosition(Math::Vector3 pos)		{ mPosition = pos;}
	void SetDirection(Math::Vector3 dir)	{ mDirection = dir;}
	void SetScale(Math::Vector3 scale)		{mScale = scale;}
	void SetRadius(float r)					{mRadius = r;}
	void SetActive(bool a)					{IsActive = a;}
	void SetMoveSpeed(float spd)			{mMoveSpeed = spd;}
	
	
protected:
	
	Math::Vector3 mPosition;
	Math::Vector3 mDirection;
	Math::Vector3 mScale;
	float mRadius;
	Graphics::StaticMeshX* mMesh;
	bool IsActive;
	float mMoveSpeed;
	
};
}
}
#endif // #ifndef GameObject_H