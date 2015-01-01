//====================================================================================================
//	Filename:		Sprite.h
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================
#include "precompiled.h"
#include "Sprite.h"
#include "CORE/Log.h"

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

Sprite::Sprite()
	:mpTexture(NULL)
	,mpSprite(NULL)
	,mWidth(0)
	,mHeight(0)
{
	mPosition.x = 0.0f;
	mPosition.y = 0.0f;
	mPosition.z = 0.0f;
	mScale.x = 1.0f;
	mScale.y = 1.0f;
}

//----------------------------------------------------------------------------------------------------

Sprite::~Sprite()
{
	Destroy();
}

//----------------------------------------------------------------------------------------------------

void Sprite::Create(IDirect3DDevice9* pDevice, char* pTextureName)
{
	//D3DXCreateTextureFromFile(pDevice, pTextureName, &mpTexture);
	D3DXIMAGE_INFO info;
	D3DXCreateTextureFromFileExA
	(
		pDevice,	// Pointer to Direct 3D device interface
		pTextureName,						  // Image filename
		D3DX_DEFAULT_NONPOW2,			   // Texture width (D3DX_DEFAULT_NONPOW2 = use texture size/no rounding)
		D3DX_DEFAULT_NONPOW2,			   // Texture height (D3DX_DEFAULT_NONPOW2 = use texture size/no rounding)
		0,								  // Mip levels (0 = full chain)
		D3DUSAGE_DYNAMIC,				   // Usage,
		D3DFMT_UNKNOWN,					 // Pixel format (D3DFMT_UNKNOWN = read from file)
		D3DPOOL_DEFAULT,					// Memory pool (D3DFMT_UNKNOWN = let D3D decide)
		D3DX_FILTER_NONE,				   // Filter
		D3DX_DEFAULT,					   // Mip-map filter
		0,								  // Color key for transparency (0 = disable)
		&info,							  // Texture info
		NULL,							   // Palette entries
		&mpTexture						 // Pointer to Direct 3D texture interface
	);
	mWidth = info.Width;
	mHeight = info.Height;
	D3DXCreateSprite(pDevice,&mpSprite);
}

//----------------------------------------------------------------------------------------------------

void Sprite::Destroy()
{
	SafeRelease(mpTexture);
	SafeRelease(mpSprite);
}

//----------------------------------------------------------------------------------------------------

void Sprite::Render()
{
	mpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	mpSprite->Draw(mpTexture,NULL,NULL,&mPosition,0xFFFFFFFF);
	mpSprite->End();
}

//----------------------------------------------------------------------------------------------------

void Sprite::GetDimension(int &width, int &height)
{
	width = mWidth * mScale.x;
	height = mHeight * mScale.y;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetPosition(int x,int y,int z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetScale(float x, float y)
{
	mScale.x = x;
	mScale.y = y;
}


}//namespace MAGE3D
}//namespace Graphics



