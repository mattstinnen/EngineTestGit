//====================================================================================================
//	Filename:		StaticMeshX.h
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================
#include "precompiled.h"
#include "StaticMeshX.h"
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

StaticMeshX::StaticMeshX()
	:pMesh(NULL) 
	,pMeshMaterials(NULL)
	,pMeshTextures(NULL)
	,mNumMaterials(NULL)

{

}

//----------------------------------------------------------------------------------------------------

StaticMeshX::~StaticMeshX()
{

}

//----------------------------------------------------------------------------------------------------

void StaticMeshX::Create(IDirect3DDevice9 *pDevice, const char *pMeshFilename, const char *pTextureFilename)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;
        // Load the mesh from the specified file
    if(FAILED(D3DXLoadMeshFromX( pMeshFilename, D3DXMESH_SYSTEMMEM, 
                                           pDevice, NULL, 
                                           &pD3DXMtrlBuffer, NULL, &mNumMaterials, 
                                            &pMesh )))
	{
		ASSERT(false,"failed to load a mesh");
	}

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pMeshMaterials = new D3DMATERIAL9[mNumMaterials];
    pMeshTextures  = new LPDIRECT3DTEXTURE9[mNumMaterials];

	 for( DWORD i=0; i<mNumMaterials; i++ )
        {
                // Copy the material
                pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
                // Set the ambient color for the material (D3DX does not do this)
                pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
                pMeshTextures[i] = NULL;
                
				if( FAILED( D3DXCreateTextureFromFile( pDevice, 
														pTextureFilename, 
                                                         &pMeshTextures[i] ) ) )
                {
                    ASSERT(false,"failed to load a texture");    
                }                   
        }

        // Done with the material buffer
        pD3DXMtrlBuffer->Release();
}

void StaticMeshX::Destroy()
{
	SafeDeleteArray(pMeshMaterials);
	SafeDeleteArray(pMeshTextures);
	SafeRelease(pMesh);
	
}
}//namespace MAGE3D
}//namespace Graphics



