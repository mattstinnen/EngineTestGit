//====================================================================================================
//	Filename:		HeightMap.cpp
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================
#include "precompiled.h"
#include "HeightMap.h"


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

HeightMap::HeightMap()
	:mpHeightData(NULL)
	,mNumColumn(0)
	,mNumRow(0)
{
	// empty
}

//----------------------------------------------------------------------------------------------------

HeightMap::~HeightMap()
{
	// clear data
	Purge();
}

//----------------------------------------------------------------------------------------------------

void HeightMap::GenerateFromFile(IDirect3DDevice9* pDevice, const char* pFileName, float maxHeight)
{
	// clear everything
	Purge();
	
	//Get image info
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(pFileName,&info);
	
	//get image dimention
	mNumColumn	= info.Width;
	mNumRow		= info.Height;

	//determine the byte offset to use
	int offset = 1;
	if(	info.Format == D3DFMT_A8R8G8B8 ||
		info.Format == D3DFMT_X8R8G8B8 ||
		info.Format == D3DFMT_A8B8G8R8 ||
		info.Format == D3DFMT_A8B8G8R8	 )
	{
		offset = 4;
	}
	
	// create a new buffer for height data
	mpHeightData = new float[info.Width * info.Height];

	//create the height map texture
	IDirect3DTexture9* pTexture = NULL;
	D3DXCreateTextureFromFileA(pDevice,pFileName,&pTexture);

	// lock the texture
	D3DLOCKED_RECT rect;
	pTexture->LockRect(0, &rect, NULL, NULL);
	
	// extract height values from the texture
	// note: Data is in left to right, top to bottom order. 
	//however direct3d  has position going bottom to top so we must flip the z values
	BYTE* pData = static_cast<BYTE*>(rect.pBits);
	for(u32 z = 0; z < info.Height; ++z)
	{
		for(u32 x = 0; x < info.Width; ++x)
		{
			const int bufferIndex = (x * offset) + ((info.Height - z - 1)* rect.Pitch);
			const int heightIndex = x + (z * info.Width);
			mpHeightData[heightIndex] = pData[bufferIndex] /255.0f * maxHeight;
		}
	}
	
	// unlock and release texture
	pTexture->UnlockRect(0);
	SafeRelease(pTexture);

}

//----------------------------------------------------------------------------------------------------

void HeightMap::Purge()
{
	SafeDeleteArray(mpHeightData);

	// reset Dimensions
	mNumColumn = 0;
	mNumRow = 0;
}

//----------------------------------------------------------------------------------------------------

float HeightMap::GetHeightEntry(u32 col, u32 row) const
{
	float height = 0.0f;
	if(col < mNumColumn && row < mNumRow)
	{
		const int index = col + (row * mNumColumn);
		height = mpHeightData[index];
	}
	return height;
}

//----------------------------------------------------------------------------------------------------




}//namespace MAGE3D
}//namespace Graphics



