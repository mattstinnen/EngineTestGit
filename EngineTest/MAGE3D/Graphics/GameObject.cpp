#include <Precompiled.h>
#include "GameObject.h"




namespace MAGE3D
{
namespace Graphics
{


//====================================================================================================
// class definition
//====================================================================================================
GameObject::GameObject()
	:mPosition(0.0f,0.0f,0.0f)
	,mDirection(0.0f,0.0f,1.0f)
	,mScale(1.0f,1.0f,1.0f)
	,mRadius(0.0f)
	,IsActive(true)
	,mMoveSpeed(20.0f)
{

}

GameObject::~GameObject()
{

}

//----------------------------------------------------------------------------------------------------

void GameObject::Load(Graphics::StaticMeshX* pMesh)
{
	mMesh = pMesh;
}

//----------------------------------------------------------------------------------------------------

void GameObject::Unload()
{
	
}

//----------------------------------------------------------------------------------------------------

void GameObject::Update(float fSeconds)
{
	if(IsActive)
	{
	
	}
}

//----------------------------------------------------------------------------------------------------

void GameObject::Render(IDirect3DDevice9* pDevice)
{
	if(IsActive)
	{
		D3DXMATRIX matRot, matTrans, matWorld, matScale;
		D3DXMatrixRotationY(&matRot,atan2(mDirection.x,mDirection.z)+ D3DX_PI);
		D3DXMatrixTranslation(&matTrans,mPosition.x,mPosition.y,mPosition.z);
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixScaling(&matScale,mScale.x,mScale.y,mScale.z);
		matWorld = matScale * matRot * matTrans;
		pDevice->SetTransform(D3DTS_WORLD,&matWorld);
		
		for( DWORD i=0; i<mMesh->GetNumOfMaterials(); i++ )
		{
				// Set the material and texture for this subset
				pDevice->SetMaterial( &mMesh->GetMaterial(i) );
				pDevice->SetTexture( 0, mMesh->GetTexture(i) );
				// Draw the mesh subset
				mMesh->DrawSubset(i);
		}
	}
}

//----------------------------------------------------------------------------------------------------

void GameObject::Walk(float distance)
{
	Math::Vector3 temp = mDirection;
	temp.Normalize();
	mPosition += (temp * distance * mMoveSpeed);
}

}
}