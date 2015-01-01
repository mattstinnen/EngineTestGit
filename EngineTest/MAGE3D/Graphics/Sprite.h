#ifndef Sprite_H
#define Sprite_H

//====================================================================================================
//	Filename:		Sprite.h
//	Created by:		Matt Stinnen
//	Discription:	a class for creating Sprites to be shown on screen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================

#include <Math/vector3.h>

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

class Sprite 
{
public:


	Sprite();
	~Sprite();
	
	void Create(IDirect3DDevice9* pDevice, char* pTextureName);
	void Destroy();
	void Render();
	void GetDimension(int &width, int &height);
	void SetPosition(int x, int y, int z);
	void SetScale(float x, float y);

private:
	
	LPDIRECT3DTEXTURE9 mpTexture;
	LPD3DXSPRITE mpSprite;
	int mWidth;
	int mHeight;
	D3DXVECTOR3 mPosition;
	D3DXVECTOR2 mScale;
};

}//namespace MAGE3D
}//namespace Graphics

#endif // Sprite_H