#ifndef StaticMeshX_H
#define StaticMeshX_H

//====================================================================================================
//	Filename:		StaticMeshX.h
//	Created by:		Matt Stinnen
//	Discription:	a class for creating StaticMeshXs to be shown on screen
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

class StaticMeshX 
{
public:


	StaticMeshX();
	~StaticMeshX();
	
	void Create(IDirect3DDevice9* pDevice, const char* pMeshFilename, const char* pTextureFilename);
	void Destroy();

	D3DMATERIAL9		GetMaterial(DWORD index)	{return pMeshMaterials[index];}
	LPDIRECT3DTEXTURE9	GetTexture(DWORD index)		{return pMeshTextures[index];}
	DWORD				GetNumOfMaterials()			{return mNumMaterials;}
	

	void DrawSubset(DWORD index) {pMesh->DrawSubset(index);}

private:
LPD3DXMESH              pMesh;          // Our mesh object in sysmem

D3DMATERIAL9*           pMeshMaterials; // Materials for our mesh

LPDIRECT3DTEXTURE9*     pMeshTextures;  // Textures for our mesh

DWORD                   mNumMaterials; // Number of mesh materials
	
};

}//namespace MAGE3D
}//namespace Graphics

#endif // StaticMeshX_H