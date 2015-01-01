#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

//====================================================================================================
//	Filename:		HeightMap.h
//	Created by:		Matt Stinnen
//	Discription:	a class for creating fonts to be shown on screen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================



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

class HeightMap 
{
public:

	HeightMap();
	virtual ~HeightMap();

	void GenerateFromFile(IDirect3DDevice9* pDevice, const char* pFileName, float maxHeight);
	//void GeneratePerlinNoise

	void Purge();

	float GetHeightEntry(u32 col, u32 row) const;
	u32 GetNumColumn() const {return mNumColumn;}
	u32 GetNumRow() const {return mNumRow;}

protected:
	float* mpHeightData;

	u32 mNumColumn;
	u32 mNumRow;
};

}//namespace MAGE3D
}//namespace Graphics

#endif // FONT_H