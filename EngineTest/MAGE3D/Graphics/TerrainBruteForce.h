#ifndef TERRAINBRUTEFORCE_H
#define TERRAINBRUTEFORCE_H

//====================================================================================================
//	Filename:		HeightMap.h
//	Created by:		Matt Stinnen
//	Discription:	a class for creating fonts to be shown on screen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================

#include "Graphics/Heightmap.h"

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

class TerrainBruteForce
{
public:

	TerrainBruteForce();
	virtual ~TerrainBruteForce();

	void Create(IDirect3DDevice9* pDevice, const char* pFileName);
	void Destroy();

	void Render(IDirect3DDevice9* pDevice);

	float GetHeight(float posX, float posZ);
	float GetWidth() {return mWidth;}
	float GetLength() {return mLength;}
	
protected:
	IDirect3DVertexBuffer9* mpVertexBuffer;
	IDirect3DIndexBuffer9* mpIndexBuffer;

	IDirect3DTexture9* mpTexture;
	IDirect3DTexture9* mpTexture2;
	IDirect3DTexture9* mpAlphaTex;


	Graphics::HeightMap mHeightMap;

	float mCellSpacing;
	float mMaxHeight;

	float mWidth;
	float mLength;
	
	float offsetX;
	float offsetZ;

	u32 mNumVerticies;
	u32 mNumTriangles;
	

};

}//namespace MAGE3D
}//namespace Graphics

#endif // TerrainBruteForce